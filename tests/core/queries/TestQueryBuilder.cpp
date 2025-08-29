#include "TestQueryBuilder.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/MediumUserData.h"
#include "model/Video.h"
#include "queries/filters/FavoriteFilter.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/sortings/TitleSort.h"

#include <QTest>
#include <ranges>
#include <unordered_set>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Video;

using Core::Queries::Filters::FavoriteFilter;
using Core::Queries::Filters::MediumTypeFilter;
using Core::Queries::Sortings::TitleSort;

using FilterGenerator = std::function<std::unique_ptr<const Filter>()>;
using SortGenerator = std::function<std::unique_ptr<const Sort>()>;
using QueryTester = std::function<void(const QueryBuilder &)>;

void TestQueryBuilder::queryTestHelper(const Library &lib, const QueryBuilder &queryBuilder,
                                       const std::vector<QUuid> &expectedIdsAfterQuery) {
    const auto actualMediaAfterQuery{queryBuilder.query(lib)};

    auto idView =
        actualMediaAfterQuery | std::views::transform([](const Medium *m) { return m->id(); });
    const std::vector<QUuid> actualIds{idView.begin(), idView.end()};
    QCOMPARE(actualIds, expectedIdsAfterQuery);
}

void TestQueryBuilder::testCopyConstruction() const {
    QueryBuilder original;
    original.addFilter(std::make_unique<MediumTypeFilter>(
        std::unordered_set<std::type_index>{typeid(Book), typeid(Article)}));
    original.addFilter(std::make_unique<FavoriteFilter>());
    original.setSort(std::make_unique<TitleSort>());

    const QueryBuilder copy{original};

    // Check that the copy has the same behavior expected from the original.
    queryTestHelper(defaultLib, copy, std::vector{bookFavoriteA.id(), articleFavoriteC.id()});
}

void TestQueryBuilder::testCopyAssignment() const {
    QueryBuilder original;
    original.addFilter(std::make_unique<MediumTypeFilter>(
        std::unordered_set<std::type_index>{typeid(Book), typeid(Article)}));
    original.addFilter(std::make_unique<FavoriteFilter>());
    original.setSort(std::make_unique<TitleSort>());

    QueryBuilder copy;
    copy = original;

    // Check that the copy has the same behavior expected from the original.
    queryTestHelper(defaultLib, copy, std::vector{bookFavoriteA.id(), articleFavoriteC.id()});
}

void TestQueryBuilder::testAddFilter_data() const {
    QTest::addColumn<QueryBuilder>("queryBuilder");
    QTest::addColumn<FilterGenerator>("filterToAddGenerator");
    QTest::addColumn<bool>("shouldBeAdded");
    QTest::addColumn<QueryTester>("queryTester");

    {
        QTest::addRow("Adding a new, previously absent filter")
            << QueryBuilder{} << FilterGenerator{[] { return std::make_unique<FavoriteFilter>(); }}
            << true << QueryTester{[this](const QueryBuilder &qb) {
                   auto expectedIdsView{
                       orderedIds | std::views::filter([this](const QUuid id) {
                           return defaultLib.getMedium(id).value()->userData().favorite();
                       })};
                   const std::vector<QUuid> expectedIds{expectedIdsView.begin(),
                                                        expectedIdsView.end()};

                   queryTestHelper(defaultLib, qb, expectedIds);
               }};
    }

    {
        QueryBuilder queryBuilder;
        queryBuilder.addFilter(
            std::make_unique<MediumTypeFilter>(MediumTypeFilter{{typeid(Book)}}));
        QTest::addRow("Replacing an old filter") << queryBuilder << FilterGenerator{[] {
            return std::make_unique<MediumTypeFilter>(MediumTypeFilter{{typeid(Article)}});
        }} << true << QueryTester{[this](const QueryBuilder &qb) {
            queryTestHelper(defaultLib, qb, std::vector{articleFavoriteC.id()});
        }};
    }

    {
        QTest::addRow("Trying to add a nullptr") << QueryBuilder{} << FilterGenerator{[] {
            return std::unique_ptr<FavoriteFilter>{nullptr};
        }} << false << QueryTester{[](const QueryBuilder &) {
            // nothing to do here, this won't be called
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

void TestQueryBuilder::testSetSort_data() const {
    QTest::addColumn<SortGenerator>("sortToSetGenerator");
    QTest::addColumn<bool>("shouldBeSet");
    QTest::addColumn<QueryTester>("queryTester");

    QTest::addRow("Setting a valid sort")
        << SortGenerator{[] { return std::make_unique<TitleSort>(); }} << true
        << QueryTester{[this](const QueryBuilder &qb) {
               queryTestHelper(defaultLib, qb,
                               std::vector{bookFavoriteA.id(), bookB.id(), articleFavoriteC.id(),
                                           videoFavoriteD.id()});
           }};

    QTest::addRow("Trying to set a nullptr")
        << SortGenerator{[] { return std::unique_ptr<TitleSort>{nullptr}; }} << false
        << QueryTester{[](const QueryBuilder &) {
               // nothing to do here, this won't be called
           }};
}
void TestQueryBuilder::testSetSort() {
    QFETCH(SortGenerator, sortToSetGenerator);
    QFETCH(bool, shouldBeSet);
    QFETCH(QueryTester, queryTester);

    QueryBuilder queryBuilder;
    std::unique_ptr<const Sort> sortToSet{sortToSetGenerator()};

    const bool wasSet{queryBuilder.setSort(std::move(sortToSet))};

    QCOMPARE(wasSet, shouldBeSet);
    if (shouldBeSet) {
        queryTester(queryBuilder);
    }
}

void TestQueryBuilder::testSetSearch_data() const {}
void TestQueryBuilder::testSetSearch() {}

void TestQueryBuilder::testQuery_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<QueryBuilder>("queryBuilder");
    QTest::addColumn<std::vector<QUuid>>("expectedQueryResultIds");

    {
        QTest::addRow("Identity query (no filters, no sort) returns all media")
            << defaultLib << QueryBuilder{} << orderedIds;
    }
    {
        QueryBuilder qb;
        qb.addFilter(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Video)}));
        QTest::addRow("Single filter query returns only matching media")
            << defaultLib << qb << std::vector{videoFavoriteD.id()};
    }
    {
        QueryBuilder qb;
        qb.addFilter(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
        qb.addFilter(std::make_unique<FavoriteFilter>());
        QTest::addRow("Multiple filters return intersection of results")
            << defaultLib << qb << std::vector{bookFavoriteA.id()};
    }
    {
        QueryBuilder qb;
        qb.setSort(std::make_unique<TitleSort>());
        QTest::addRow("Sort-only query reorders all media")
            << defaultLib << qb
            << std::vector{bookFavoriteA.id(), bookB.id(), articleFavoriteC.id(),
                           videoFavoriteD.id()};
    }
    {
        QueryBuilder qb;
        qb.addFilter(
            std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
        qb.setSort(std::make_unique<TitleSort>());
        QTest::addRow("Filter and sort returns sorted subset of media")
            << defaultLib << qb << std::vector{bookFavoriteA.id(), bookB.id()};
    }
}
void TestQueryBuilder::testQuery() {}

void TestQueryBuilder::testReset() const {
    QueryBuilder qb;
    qb.addFilter(std::make_unique<FavoriteFilter>());
    qb.setSort(std::make_unique<TitleSort>());

    qb.reset();

    queryTestHelper(defaultLib, qb, orderedIds);
}