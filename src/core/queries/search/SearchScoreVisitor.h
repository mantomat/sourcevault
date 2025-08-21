#ifndef SEARCHSCOREVISITOR_H
#define SEARCHSCOREVISITOR_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/ValidatedField.h"
#include "model/Video.h"
#include "shared/MediumVisitor.h"

#include <QString>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::Video;

using Core::Model::ValidatedField;
using Core::Model::ValidatedSet;
using Core::Shared::MediumVisitor;

namespace Core::Queries::Search {

struct SearchWeights {
    static constexpr double defaultContiguity{0.6};
    static constexpr double defaultIdentifiers{100.0};
    static constexpr double defaultFields{60.0};

    double contiguityBias{defaultContiguity};

    double identifiers{defaultIdentifiers};
    double fields{defaultFields};
};

struct SearchOptions {
    QString dateFormat{"yyyyMMdd"};
};

class SearchScoreVisitor : public MediumVisitor {

    QString searchTerm;

    SearchWeights weights;
    SearchOptions options;

    double score{0.0};

  public:
    SearchScoreVisitor(const SearchScoreVisitor &) = default;

    explicit SearchScoreVisitor(QString newSearchTerm = QString{},
                                SearchWeights newWeights = SearchWeights{},
                                SearchOptions newOptions = SearchOptions{});

    SearchScoreVisitor(SearchScoreVisitor &&) = default;
    auto operator=(const SearchScoreVisitor &) -> SearchScoreVisitor & = default;
    auto operator=(SearchScoreVisitor &&) -> SearchScoreVisitor & = default;
    ~SearchScoreVisitor() override = default;

    [[nodiscard]] auto getScore() const -> double;

    void visit(const Book &book) override;
    void visit(const Article &article) override;
    void visit(const Video &video) override;

  private:
    struct LcsResult {
        size_t lcsLength;
        size_t maxContiguousRun;
    };

    /**
     * @brief Calculates the length of the Longest Common Substring and the length of the longest
     * contiguous run inside it.
     */
    [[nodiscard]] static auto lcsWithContiguityBonus(const QString &x, const QString &y)
        -> LcsResult;

    /**
     * @brief Returns a score in [0, 1] based on the Longest Common Subsequence between
     * `searchTerm` and `field`.
     */
    [[nodiscard]] auto computeRawScore(const QString &field) const -> double;

    /**
     * @brief Utility function to update the score with a field.
     */
    template <typename T>
    void scoreField(
        double weight, const ValidatedField<T> &field,
        const std::function<QString(const ValidatedField<T> &)> &stringGetter =
            [](const ValidatedField<T> &field) { return field.get().value(); }) {
        if (!field.has()) {
            return;
        }
        score = std::max(score, computeRawScore(stringGetter(field)) * weight);
    }

    /**
     * @brief Utility function to update the score with a set.
     */
    void scoreSet(double weight, const ValidatedSet<QString> &set);

    /**
     * @brief Utility function to update the score with common Medium attributes.
     */
    void scoreMediumFields(const Medium &medium);
};

}

#endif