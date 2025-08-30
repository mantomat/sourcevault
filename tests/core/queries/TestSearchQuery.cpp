#include "TestSearchQuery.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "queries/FilteringQuery.h"
#include "queries/SearchQuery.h"
#include "queries/filters/MediumTypeFilter.h"
#include "queries/search/SearchEngine.h"

#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Queries::FilteringQuery;
using Core::Queries::SearchQuery;
using Core::Queries::Filters::MediumTypeFilter;
using Core::Queries::Search::SearchEngine;

using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;

void TestSearchQuery::testQuery_data() {
    QTest::addColumn<SearchQuery>("query");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<std::vector<QUuid>>("expectedIds");

    const Article article{Article::create("aa").value()};
    const Book book{Book::create("ba").value()};
    const Video video{Video::create("va").value()};
    const MediaGenerator bookArticleVideoGenerator{[book, article, video] {
        std::vector<std::unique_ptr<const Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        media.push_back(std::make_unique<Article>(article));
        media.push_back(std::make_unique<Video>(video));
        return media;
    }};

    auto emptyFilteringQuery{FilteringQuery::create({}).value()};
    QTest::addRow(
        "A search query with no filtering and empty search term doesn't modify the vector")
        << SearchQuery{emptyFilteringQuery, SearchEngine{""}} << bookArticleVideoGenerator
        << std::vector{book.id(), article.id(), video.id()};

    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(std::make_unique<MediumTypeFilter>(
        std::unordered_set<std::type_index>{typeid(Book), typeid(Article)}));
    auto bookArticleFilteringQuery{FilteringQuery::create(std::move(filters)).value()};
    QTest::addRow(
        "A search query with non-empty filtering and empty search term just filters the vector")
        << SearchQuery{bookArticleFilteringQuery, SearchEngine{""}} << bookArticleVideoGenerator
        << std::vector{book.id(), article.id()};

    QTest::addRow("A search query with empty filtering and non-empty search term just searches "
                  "through the vector")
        << SearchQuery{emptyFilteringQuery, SearchEngine{"b"}} << bookArticleVideoGenerator
        << std::vector{book.id()};

    QTest::addRow("A search query with non-empty filtering and non-empty search term first filters "
                  "and then searches through the vector")
        << SearchQuery{bookArticleFilteringQuery, SearchEngine{"a"}} << bookArticleVideoGenerator
        << std::vector{article.id(), book.id()};
};
void TestSearchQuery::testQuery() {
    QFETCH(SearchQuery, query);
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