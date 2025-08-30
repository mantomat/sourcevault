#include "TestFilteringQuery.h"

#include "model/Article.h"
#include "model/Book.h"
#include "queries/FilteringQuery.h"
#include "queries/filters/Filter.h"

#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Queries::FilteringQuery;
using Core::Queries::Filters::Filter;

using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;
using FilterGenerator = std::function<std::unique_ptr<const Filter>()>;
using FiltersGenerator = std::function<std::vector<std::unique_ptr<const Filter>>()>;

class MockFilter : public Filter {
    bool doesMatch;

  public:
    explicit MockFilter(bool matches)
        : doesMatch{matches} {}
    [[nodiscard]] auto matches(const Medium *medium) const -> bool override {
        return medium != nullptr && doesMatch;
    }
    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override {
        return std::make_unique<MockFilter>(doesMatch);
    }
};

class AnotherMockFilter : public Filter {
    bool doesMatch;

  public:
    explicit AnotherMockFilter(bool matches)
        : doesMatch{matches} {}
    [[nodiscard]] auto matches(const Medium *medium) const -> bool override {
        return medium != nullptr && doesMatch;
    }
    [[nodiscard]] auto clone() const -> std::unique_ptr<Filter> override {
        return std::make_unique<AnotherMockFilter>(doesMatch);
    }
};

void TestFilteringQuery::testCopyConstructor() {
    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(std::make_unique<MockFilter>(true));
    const auto original{FilteringQuery::create(std::move(filters)).value()};

    const FilteringQuery copy{original};

    const Book book{Book::create("test book").value()};
    std::vector<const Medium *> media{&book};

    auto originalResult{original.query(media)};
    auto copyResult{copy.query(media)};

    QCOMPARE(originalResult, copyResult);
}

void TestFilteringQuery::testCopyAssignment() {
    std::vector<std::unique_ptr<const Filter>> filters;
    filters.push_back(std::make_unique<MockFilter>(true));
    const auto original{FilteringQuery::create(std::move(filters)).value()};

    auto copy{FilteringQuery::create({}).value()};
    copy = original;

    const Book book{Book::create("test book").value()};
    std::vector<const Medium *> media{&book};

    auto originalResult{original.query(media)};
    auto copyResult{copy.query(media)};

    QCOMPARE(originalResult, copyResult);
}

void TestFilteringQuery::testCreate_data() {
    QTest::addColumn<FiltersGenerator>("filtersGenerator");
    QTest::addColumn<bool>("shouldBeCreated");

    QTest::newRow("Vectors with multiple different filters are valid") << FiltersGenerator{[] {
        std::vector<std::unique_ptr<const Filter>> filters{};
        filters.push_back(std::make_unique<MockFilter>(true));
        filters.push_back(std::make_unique<AnotherMockFilter>(false));
        return filters;
    }} << true;

    QTest::newRow("Empty vectors are valid")
        << FiltersGenerator{[] { return std::vector<std::unique_ptr<const Filter>>{}; }} << true;

    QTest::newRow("Vectors containing nullptrs are invalid") << FiltersGenerator{[] {
        std::vector<std::unique_ptr<const Filter>> filters{};
        filters.push_back(std::make_unique<MockFilter>(true));
        filters.push_back(nullptr);
        return filters;
    }} << false;

    QTest::newRow("Vectors containing duplicates are invalid") << FiltersGenerator{[] {
        std::vector<std::unique_ptr<const Filter>> filters{};
        filters.push_back(std::make_unique<MockFilter>(true));
        filters.push_back(std::make_unique<MockFilter>(false));
        return filters;
    }} << false;
}

void TestFilteringQuery::testCreate() {
    QFETCH(FiltersGenerator, filtersGenerator);
    QFETCH(bool, shouldBeCreated);
    auto filters{filtersGenerator()};

    auto query{FilteringQuery::create(std::move(filters))};

    QCOMPARE(query.has_value(), shouldBeCreated);
}

void TestFilteringQuery::testQuery_data() {
    QTest::addColumn<FilteringQuery>("query");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<size_t>("expectedCount");

    MediaGenerator mediaGenerator{[] {
        auto vec = std::vector<std::unique_ptr<const Medium>>{};
        vec.push_back(std::make_unique<Book>(Book::create("A Book").value()));
        vec.push_back(std::make_unique<Article>(Article::create("An Article").value()));
        return vec;
    }};

    QTest::newRow("No filters should return all media")
        << FilteringQuery::create({}).value() << mediaGenerator << size_t{2};

    std::vector<std::unique_ptr<const Filter>> oneFilterTrue;
    oneFilterTrue.push_back(std::make_unique<MockFilter>(true));
    QTest::newRow("One filter that matches everything")
        << FilteringQuery::create(std::move(oneFilterTrue)).value() << mediaGenerator << size_t{2};

    std::vector<std::unique_ptr<const Filter>> oneFilterFalse;
    oneFilterFalse.push_back(std::make_unique<MockFilter>(false));
    QTest::newRow("One filter that matches nothing")
        << FilteringQuery::create(std::move(oneFilterFalse)).value() << mediaGenerator << size_t{0};

    std::vector<std::unique_ptr<const Filter>> twoFiltersTrue;
    twoFiltersTrue.push_back(std::make_unique<MockFilter>(true));
    twoFiltersTrue.push_back(std::make_unique<AnotherMockFilter>(true));
    QTest::newRow("Two filters that both match")
        << FilteringQuery::create(std::move(twoFiltersTrue)).value() << mediaGenerator << size_t{2};

    std::vector<std::unique_ptr<const Filter>> twoFiltersMix;
    twoFiltersMix.push_back(std::make_unique<MockFilter>(true));
    twoFiltersMix.push_back(std::make_unique<AnotherMockFilter>(false));
    QTest::newRow("One filter matches, one does not")
        << FilteringQuery::create(std::move(twoFiltersMix)).value() << mediaGenerator << size_t{0};
}

void TestFilteringQuery::testQuery() {
    QFETCH(FilteringQuery, query);
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(size_t, expectedCount);

    auto media{mediaGenerator()};
    auto ptrsView{media | std::views::transform([](const auto &m) { return m.get(); })};
    std::vector<const Medium *> mediaPtrs{ptrsView.begin(), ptrsView.end()};

    const auto result{query.query(mediaPtrs)};

    QCOMPARE(result.size(), expectedCount);
}
