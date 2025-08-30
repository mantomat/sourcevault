#include "TestSortingQuery.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "queries/FilteringQuery.h"
#include "queries/SortingQuery.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/sortings/DateAddedSort.h"
#include "queries/sortings/Sort.h"
#include "queries/sortings/TitleSort.h"

#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Queries::FilteringQuery;
using Core::Queries::SortingQuery;
using Core::Queries::Filters::MediumTypeFilter;
using Core::Queries::Sortings::DateAddedSort;
using Core::Queries::Sortings::Sort;
using Core::Queries::Sortings::TitleSort;

using SortGenerator = std::function<std::unique_ptr<const Sort>()>;
using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;

void TestSortingQuery::testCopyConstruction() {
    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(
        std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
    auto filteringQuery{FilteringQuery::create(std::move(filters)).value()};
    const auto original{
        SortingQuery::create(std::move(filteringQuery), std::make_unique<TitleSort>()).value()};

    const SortingQuery copy{original};

    const Article article{Article::create("a").value()};
    const Book book1{Book::create("b").value()};
    const Book book2{Book::create("c").value()};

    std::vector<const Medium *> media{&article, &book2, &book1};

    auto originalResult{original.query(media)};
    auto copyResult{copy.query(media)};

    QCOMPARE(originalResult, copyResult);
}

void TestSortingQuery::testCopyAssignment() {
    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(
        std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Book)}));
    auto filteringQuery{FilteringQuery::create(std::move(filters)).value()};
    const auto original{
        SortingQuery::create(std::move(filteringQuery), std::make_unique<TitleSort>()).value()};

    auto copy{
        SortingQuery::create(FilteringQuery::create({}).value(), std::make_unique<DateAddedSort>())
            .value()};
    copy = original;

    const Article article{Article::create("a").value()};
    const Book book1{Book::create("b").value()};
    const Book book2{Book::create("c").value()};

    std::vector<const Medium *> media{&article, &book2, &book1};

    auto originalResult{original.query(media)};
    auto copyResult{copy.query(media)};

    QCOMPARE(originalResult, copyResult);
}

void TestSortingQuery::testCreate_data() {
    QTest::addColumn<FilteringQuery>("queryParameter");
    QTest::addColumn<SortGenerator>("sortParameterGenerator");
    QTest::addColumn<bool>("shouldCreate");

    QTest::addRow("Valid FilteringQuery and Sort pointer do create a SortingQuery")
        << FilteringQuery::create({}).value()
        << SortGenerator{[] { return std::make_unique<TitleSort>(); }} << true;

    QTest::addRow("Sort nullptr fails")
        << FilteringQuery::create({}).value()
        << SortGenerator{[] { return std::unique_ptr<TitleSort>{nullptr}; }} << false;
}
void TestSortingQuery::testCreate() {
    QFETCH(FilteringQuery, queryParameter);
    QFETCH(SortGenerator, sortParameterGenerator);
    QFETCH(bool, shouldCreate);

    const auto sortingQueryOpt{SortingQuery::create(queryParameter, sortParameterGenerator())};

    QCOMPARE(sortingQueryOpt.has_value(), shouldCreate);
}

void TestSortingQuery::testQuery_data() {
    QTest::addColumn<SortingQuery>("query");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<std::vector<QUuid>>("expectedIds");

    const Book book{Book::create("c").value()};
    const Article article{Article::create("b").value()};
    const Video video{Video::create("a").value()};
    const MediaGenerator bookArticleVideoGenerator{[book, article, video] {
        std::vector<std::unique_ptr<const Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        media.push_back(std::make_unique<Article>(article));
        media.push_back(std::make_unique<Video>(video));
        return media;
    }};

    auto emptyFilteringQuery{FilteringQuery::create({}).value()};
    QTest::addRow("SortingQuery with empty filtering just sorts")
        << SortingQuery::create(emptyFilteringQuery, std::make_unique<TitleSort>()).value()
        << bookArticleVideoGenerator << std::vector{video.id(), article.id(), book.id()};

    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(std::make_unique<MediumTypeFilter>(
        std::unordered_set<std::type_index>{typeid(Book), typeid(Article)}));
    auto bookArticleFilteringQuery{FilteringQuery::create(std::move(filters)).value()};
    QTest::addRow("SortingQuery with non-empty filtering filters and sorts")
        << SortingQuery::create(bookArticleFilteringQuery, std::make_unique<TitleSort>()).value()
        << bookArticleVideoGenerator << std::vector{article.id(), book.id()};
}
void TestSortingQuery::testQuery() {
    QFETCH(SortingQuery, query);
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(std::vector<QUuid>, expectedIds);

    auto media{mediaGenerator()};
    auto mediaPtrView{media |
                      std::views::transform([](const auto &medium) { return medium.get(); })};
    const std::vector<const Medium *> mediaPtrs{mediaPtrView.begin(), mediaPtrView.end()};

    auto queryResult{query.query(mediaPtrs)};
    auto queryResultIdsView{
        queryResult | std::views::transform([](const auto &mediumPtr) { return mediumPtr->id(); })};
    const std::vector<QUuid> queryResultIds{queryResultIdsView.begin(), queryResultIdsView.end()};

    QCOMPARE(queryResultIds, expectedIds);
}