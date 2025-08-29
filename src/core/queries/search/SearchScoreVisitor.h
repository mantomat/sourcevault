#ifndef SEARCHSCOREVISITOR_H
#define SEARCHSCOREVISITOR_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/ValidatedField.h"
#include "model/Video.h"
#include "queries/search/FieldScoreCalculator.h"
#include "shared/MediumVisitor.h"

#include <QString>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::Video;

using Core::Model::ValidatedField;
using Core::Model::ValidatedSet;
using Core::Shared::MediumVisitor;

class TestSearchScoreVisitor;
class FieldScoreCalculatorRepl;

namespace Core::Queries::Search {

struct FieldWeightLevels {
    static constexpr double defaultIdentifiers{100.0};
    double identifiers{defaultIdentifiers};
    static constexpr double defaultFields{90.0};
    double fields{defaultFields};
};

struct SearchOptions {
    QString dateFormat{"yyyy-MM-dd"};
};

class SearchScoreVisitor : public MediumVisitor {

    FieldScoreCalculator scoreCalculator;

    FieldWeightLevels weights;
    SearchOptions options;

    double score{0.0};

  public:
    explicit SearchScoreVisitor(QString newSearchTerm, SearchOptions newOptions,
                                FieldWeightLevels newWeights,
                                ScoreCalculatorConfigs newScoreConfigs);

    [[nodiscard]] auto getScore() const -> double;

    void visit(const Book &book) override;
    void visit(const Article &article) override;
    void visit(const Video &video) override;

  private:
    void updateScoreIfMax(double fieldScore);

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
        updateScoreIfMax(scoreCalculator.getWeightedScore(stringGetter(field), weight));
    }

    /**
     * @brief Utility function to update the score with a set.
     */
    void scoreSet(double weight, const ValidatedSet<QString> &set);

    /**
     * @brief Utility function to update the score with common Medium attributes.
     */
    void scoreCommonFields(const Medium &medium);
};

}

#endif