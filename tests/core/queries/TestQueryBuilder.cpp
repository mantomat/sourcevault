#include "TestQueryBuilder.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/MediumUserData.h"
#include "model/Video.h"
#include "queries/filters/FavoriteFilter.h"
#include "queries/filters/MediumTypeFilter.h"

#include <QTest>
#include <ranges>
#include <unordered_set>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Video;

using FilterGenerator = std::function<std::unique_ptr<const Filter>()>;
using QueryTester = std::function<void(const QueryBuilder &)>;

using Core::Queries::Filters::FavoriteFilter;
using Core::Queries::Filters::MediumTypeFilter;

void TestQueryBuilder::queryTestHelper(std::vector<std::unique_ptr<const Medium>> libraryMedia,
                                       const QueryBuilder &queryBuilder,
                                       const std::set<QUuid> &expectedIdsAfterQuery) {
    Library lib;
    lib.setMedia(std::move(libraryMedia));

    const auto actualMediaAfterQuery{queryBuilder.query(lib)};

    QCOMPARE(actualMediaAfterQuery.size(), expectedIdsAfterQuery.size());
    auto idView =
        actualMediaAfterQuery | std::views::transform([](const Medium *m) { return m->id(); });
    const std::set<QUuid> actualIds{idView.begin(), idView.end()};
    QCOMPARE(actualIds, expectedIdsAfterQuery);
}

void TestQueryBuilder::testCopyConstruction() {}

void TestQueryBuilder::testCopyAssignment() {}

void TestQueryBuilder::testAddFilter_data() {
    QTest::addColumn<QueryBuilder>("queryBuilder");
    QTest::addColumn<FilterGenerator>("filterToAddGenerator");
    QTest::addColumn<bool>("shouldBeAdded");
    QTest::addColumn<QueryTester>("queryTester");

    {
        QTest::addRow("Adding a new, previously absent filter")
            << QueryBuilder{} << FilterGenerator{[] { return std::make_unique<FavoriteFilter>(); }}
            << true << QueryTester{[](const QueryBuilder &qb) {
                   auto favoriteBook{Book::create("favorite book").value()};
                   favoriteBook.userData().favorite() = true;

                   std::vector<std::unique_ptr<const Medium>> media;
                   media.push_back(std::make_unique<Book>(favoriteBook));
                   media.push_back(std::make_unique<Book>(Book::create("useless book").value()));

                   queryTestHelper(std::move(media), qb, std::set{favoriteBook.id()});
               }};
    }

    {

        QueryBuilder queryBuilder;
        MediumTypeVisitor visitor{{typeid(Book)}};
        queryBuilder.addFilter(
            std::make_unique<MediumTypeFilter>(MediumTypeFilter{{typeid(Book)}}));

        QTest::addRow("Replacing an old filter") << queryBuilder << FilterGenerator{[] {
            return std::make_unique<MediumTypeFilter>(MediumTypeFilter{{typeid(Article)}});
        }} << true << QueryTester{[](const QueryBuilder &qb) {
            auto article{Article::create("article").value()};

            std::vector<std::unique_ptr<const Medium>> media;
            media.push_back(std::make_unique<Book>(Book::create("book").value()));
            media.push_back(std::make_unique<Article>(article));

            queryTestHelper(std::move(media), qb, std::set{article.id()});
        }};
    }

    {

        QTest::addRow("Trying to add a nullptr") << QueryBuilder{} << FilterGenerator{[] {
            return std::unique_ptr<FavoriteFilter>{nullptr};
        }} << false << QueryTester{[](const QueryBuilder &) {
            // nothing to do here
        }};
    }
}

void TestQueryBuilder::testAddFilter() {
    QFETCH(QueryBuilder, queryBuilder);
    QFETCH(FilterGenerator, filterToAddGenerator);
    QFETCH(bool, shouldBeAdded);
    QFETCH(QueryTester, queryTester);

    std::unique_ptr<const Filter> filterToAdd{filterToAddGenerator()};

    const bool wasAdded{queryBuilder.addFilter(std::move(filterToAdd))};

    QCOMPARE(wasAdded, shouldBeAdded);
    if (shouldBeAdded) {
        queryTester(queryBuilder);
    }
}

void TestQueryBuilder::testSetSort_data() {}
void TestQueryBuilder::testSetSort() {}

void TestQueryBuilder::testReset_data() {}
void TestQueryBuilder::testReset() {}

void TestQueryBuilder::testQuery_data() {}
void TestQueryBuilder::testQuery() {}