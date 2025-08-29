#include "SearchScoreVisitor.h"

namespace Core::Queries::Search {

SearchScoreVisitor::SearchScoreVisitor(QString newSearchTerm, SearchOptions newOptions,
                                       FieldWeightLevels newWeights,
                                       ScoreCalculatorConfigs newScoreConfigs)
    : scoreCalculator{std::move(newSearchTerm), newScoreConfigs}
    , weights{newWeights}
    , options{std::move(newOptions)} {}

auto SearchScoreVisitor::getScore() const -> double {
    return score;
}

void SearchScoreVisitor::visit(const Book &book) {
    score = 0.0;
    scoreCommonFields(book);

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
    scoreCommonFields(article);

    scoreField<QUrl>(weights.identifiers, article.articleUrl(),
                     [](const auto &field) { return field.get().value().toString(); });

    scoreField(weights.fields, article.sourceName());
    scoreField<QDate>(weights.fields, article.publicationDate(), [this](const auto &field) {
        return field.get().value().toString(options.dateFormat);
    });
}

void SearchScoreVisitor::visit(const Video &video) {
    score = 0.0;
    scoreCommonFields(video);

    scoreField<QUrl>(weights.identifiers, video.videoUrl(),
                     [](const auto &field) { return field.get().value().toString(); });

    scoreField<QDate>(weights.fields, video.uploadDate(), [this](const auto &field) {
        return field.get().value().toString(options.dateFormat);
    });
}

void SearchScoreVisitor::updateScoreIfMax(double fieldScore) {
    score = std::max(score, fieldScore);
}

void SearchScoreVisitor::scoreSet(double weight, const ValidatedSet<QString> &set) {
    if (!set.has()) {
        return;
    }
    std::ranges::for_each(set.get().value(), [this, weight](const QString &element) {
        updateScoreIfMax(scoreCalculator.getWeightedScore(element, weight));
    });
}

void SearchScoreVisitor::scoreCommonFields(const Medium &medium) {
    updateScoreIfMax(scoreCalculator.getWeightedScore(medium.title(), weights.identifiers));

    updateScoreIfMax(scoreCalculator.getWeightedScore(
        medium.dateAdded().toString(options.dateFormat), weights.fields));

    scoreField(weights.fields, medium.language());
    scoreField(weights.fields, medium.userData().notes());
    scoreSet(weights.fields, medium.authors());
}
}