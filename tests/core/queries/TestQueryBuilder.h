
#ifndef TESTQUERYBUILDER_H
#define TESTQUERYBUILDER_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/MediumUserData.h"
#include "model/Video.h"
#include "queries/QueryBuilder.h"
#include "queries/filters/Filter.h"
#include "queries/sortings/Sort.h"

#include <QObject>
#include <ranges>

using Core::Model::MediumUserData;
using Core::Queries::QueryBuilder;
using Core::Queries::Filters::Filter;
using Core::Queries::Sortings::Sort;

class TestQueryBuilder : public QObject {
    Q_OBJECT

    // Common utility data for testing
    Book bookFavoriteA{[] {
        auto book{Book::create("a").value()};
        book.userData().favorite() = true;
        return book;
    }()};
    Book bookB{[] { return Book::create("b").value(); }()};
    Article articleFavoriteC{[] {
        auto article{Article::create("c").value()};
        article.userData().favorite() = true;
        return article;
    }()};
    Video videoFavoriteD{[] {
        auto video{Video::create("d").value()};
        video.userData().favorite() = true;
        return video;
    }()};

    Library defaultLib{[this] {
        Library lib;
        lib.addMedium(std::make_unique<Book>(bookFavoriteA));
        lib.addMedium(std::make_unique<Book>(bookB));
        lib.addMedium(std::make_unique<Article>(articleFavoriteC));
        lib.addMedium(std::make_unique<Video>(videoFavoriteD));
        return lib;
    }()};

    // This follows the default library id order
    std::vector<QUuid> orderedIds{[this] {
        const std::vector libMedia{defaultLib.getMedia()};
        const auto libMediaIdView{libMedia |
                                  std::views::transform([](const auto mp) { return mp->id(); })};
        return std::vector<QUuid>{libMediaIdView.begin(), libMediaIdView.end()};
    }()};

    static void queryTestHelper(const Library &lib, const QueryBuilder &queryBuilder,
                                const std::vector<QUuid> &expectedIdsAfterQuery);

  private slots:
    void testCopyConstruction() const;

    void testCopyAssignment() const;

    void testAddFilter_data() const;
    static void testAddFilter();

    void testSetSort_data() const;
    static void testSetSort();

    void testReset() const;

    void testQuery_data() const;
    static void testQuery();

    // TODO test setSearch and query with search
};

#endif
