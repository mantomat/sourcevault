#include "TestSearchEngine.h"

#include "queries/search/SearchEngine.h"

#include <QTest>
#include <qtestcase.h>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Queries::Search::SearchEngine;
using Core::Queries::Search::SearchEngineOptions;

using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;

void TestSearchEngine::testSearch_data() {
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<SearchEngine>("searchEngine");
    QTest::addColumn<MediaGenerator>("expectedMediaGenerator");

    const Book book{Book::create("book title").value()};
    const Article article{Article::create("title article").value()};

    MediaGenerator articleAndBook{MediaGenerator{[book, article] {
        std::vector<std::unique_ptr<const Medium>> vec;
        vec.push_back(std::make_unique<Article>(article));
        vec.push_back(std::make_unique<Book>(book));
        return vec;
    }}};
    QTest::addRow("A search engine with empty search term doesn't modify the input vector")
        << articleAndBook << SearchEngine{""} << articleAndBook;

    QTest::addRow("Searching through an empty vector always returns an empty vector")
        << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }}
        << SearchEngine{"anything"}
        << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }};

    QTest::addRow("Searching through a vector with single Medium and finding it")
        << MediaGenerator{[book] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }}
        << SearchEngine{book.title()} << MediaGenerator{[book] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }};

    QTest::addRow("Searching through a vector with single Medium and not finding it")
        << MediaGenerator{[book] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }}
        << SearchEngine{"zzz"}
        << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }};

    QTest::addRow("Finding more than one Medium, reordering them (`title` comes first in article)")
        << MediaGenerator{[book, article] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Article>(article));
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }}
        << SearchEngine{"title"} << MediaGenerator{[book, article] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Article>(article));
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }};

    QTest::addRow("Finding more than one Medium, limiting to first result")
        << MediaGenerator{[book, article] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Article>(article));
               vec.push_back(std::make_unique<Book>(book));
               return vec;
           }}
        << SearchEngine{"title",
                        SearchEngineOptions{.maxResults = 1,
                                            .scoreThreshold =
                                                SearchEngineOptions::defaultScoreThreshold}}
        << MediaGenerator{[article] {
               std::vector<std::unique_ptr<const Medium>> vec;
               vec.push_back(std::make_unique<Article>(article));
               return vec;
           }};
}

void TestSearchEngine::testSearch() {
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(SearchEngine, searchEngine);
    QFETCH(MediaGenerator, expectedMediaGenerator);

    const auto media{mediaGenerator()};
    auto mediaPtrsView{media | std::views::transform([](const auto &m) { return m.get(); })};
    const std::vector<const Medium *> mediaPtrs{mediaPtrsView.begin(), mediaPtrsView.end()};

    const std::vector result{searchEngine.search(mediaPtrs)};

    const auto expectedMedia{expectedMediaGenerator()};

    QCOMPARE(result.size(), expectedMedia.size());
    for (size_t i{0}; i < result.size(); i++) {
        QCOMPARE(result[i]->id(), expectedMedia[i]->id());
    }
}