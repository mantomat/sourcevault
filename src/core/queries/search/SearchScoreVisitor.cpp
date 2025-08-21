#include "SearchScoreVisitor.h"

namespace Core::Queries::Search {

auto SearchScoreVisitor::lcsWithContiguityBonus(const QString &x, const QString &y) -> LcsResult {
    const auto rows = static_cast<size_t>(x.length() + 1);
    const auto cols = static_cast<size_t>(y.length() + 1);

    std::vector<size_t> previousLcsRow(cols, 0);
    std::vector<size_t> currentLcsRow(cols, 0);

    std::vector<size_t> previousRunRow(cols, 0);
    std::vector<size_t> currentRunRow(cols, 0);

    size_t maxContintiguousRun = 0;

    for (size_t i = 1; i < rows; ++i) {
        for (size_t j = 1; j < cols; ++j) {
            if (x[static_cast<qsizetype>(i - 1)] == y[static_cast<qsizetype>(j - 1)]) {
                currentLcsRow[j] = 1 + previousLcsRow[j - 1];

                currentRunRow[j] = 1 + previousRunRow[j - 1];
                maxContintiguousRun = std::max(maxContintiguousRun, currentRunRow[j]);

            } else {
                currentLcsRow[j] = std::max(currentLcsRow[j - 1], previousLcsRow[j]);
                currentRunRow[j] = 0;
            }
        }
        std::swap(previousLcsRow, currentLcsRow);
        std::swap(previousRunRow, currentRunRow);
    }

    return LcsResult{.lcsLength = previousLcsRow[cols - 1],
                     .maxContiguousRun = maxContintiguousRun};
}

auto SearchScoreVisitor::computeRawScore(const QString &field) const -> double {
    if (searchTerm.isEmpty() || field.isEmpty()) {
        return 0.0;
    }

    const auto [lcsLength,
                maxContiguousRun]{lcsWithContiguityBonus(searchTerm.toLower(), field.toLower())};

    const double lcsScore =
        static_cast<double>(lcsLength) / static_cast<double>(searchTerm.length());
    const double contiguityScore =
        static_cast<double>(maxContiguousRun) / static_cast<double>(searchTerm.length());

    return (lcsScore * (1 - weights.contiguityBias)) + (contiguityScore * weights.contiguityBias);
}

void SearchScoreVisitor::scoreSet(double weight, const ValidatedSet<QString> &set) {
    if (!set.has()) {
        return;
    }
    std::ranges::for_each(set.get().value(), [this, weight](const QString &element) {
        score = std::max(score, computeRawScore(element) * weight);
    });
}

void SearchScoreVisitor::scoreMediumFields(const Medium &medium) {
    score = std::max(score, computeRawScore(medium.title()) * weights.identifiers);

    score = std::max(score, computeRawScore(medium.dateAdded().toString(options.dateFormat)) *
                                weights.fields);

    scoreField(weights.fields, medium.language());
    scoreField(weights.fields, medium.userData().notes());
    scoreSet(weights.fields, medium.authors());
}

SearchScoreVisitor::SearchScoreVisitor(QString newSearchTerm, SearchWeights newWeights,
                                       SearchOptions newOptions)
    : searchTerm{std::move(newSearchTerm)}
    , weights{newWeights}
    , options{std::move(newOptions)} {}

auto SearchScoreVisitor::getScore() const -> double {
    return score;
}

void SearchScoreVisitor::visit(const Book &book) {
    score = 0.0;
    if (searchTerm.isEmpty()) {
        return;
    }
    scoreMediumFields(book);

    scoreField(weights.identifiers, book.isbn());

    scoreField(weights.fields, book.edition());
    scoreField(weights.fields, book.publisher());
    scoreField(weights.fields, book.description());
    scoreField<int>(weights.fields, book.yearPublished(), [](const ValidatedField<int> &field) {
        return QString::number(field.get().value());
    });
}

void SearchScoreVisitor::visit(const Article &article) {
    score = 0.0;
    if (searchTerm.isEmpty()) {
        return;
    }
    scoreMediumFields(article);

    scoreField<QUrl>(weights.identifiers, article.articleUrl(),
                     [](const auto &field) { return field.get().value().toString(); });

    scoreField(weights.fields, article.sourceName());
    scoreField<QDate>(weights.fields, article.publicationDate(), [this](const auto &field) {
        return field.get().value().toString(options.dateFormat);
    });
}

void SearchScoreVisitor::visit(const Video &video) {
    score = 0.0;
    if (searchTerm.isEmpty()) {
        return;
    }
    scoreMediumFields(video);

    scoreField<QUrl>(weights.identifiers, video.videoUrl(),
                     [](const auto &field) { return field.get().value().toString(); });
    scoreField<QDate>(weights.fields, video.uploadDate(), [this](const auto &field) {
        return field.get().value().toString(options.dateFormat);
    });
}

}