#include "TestDateAddedSort.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "queries/sortings/DateAddedSort.h"

#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Queries::Sortings::DateAddedSort;

using MediaGenerator = std::function<std::vector<std::unique_ptr<Medium>>()>;

void TestDateAddedSort::testConstructorAndIsAscending() {
    const DateAddedSort asc{};
    QCOMPARE(asc.isAscending(), true);

    const DateAddedSort desc{false};
    QCOMPARE(desc.isAscending(), false);
}

void TestDateAddedSort::testClone() {
    const auto original{std::make_unique<DateAddedSort>(false)};

    const auto clone{original->clone()};

    QVERIFY(original != clone);
    QVERIFY(dynamic_cast<const DateAddedSort *>(clone.get()) != nullptr);
    QCOMPARE(original->isAscending(), clone->isAscending());
}

void TestDateAddedSort::testApply_data() {
    QTest::addColumn<DateAddedSort>("sort");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<std::vector<QUuid>>("expectedIdsSorting");

    const auto firstMedium{Book::create("book", QUuid::createUuid(), QDate{2020, 1, 1}).value()};
    const auto secondMedium{
        Article::create("article", QUuid::createUuid(), QDate{2021, 1, 1}).value()};
    const auto thirdMedium{Video::create("video", QUuid::createUuid(), QDate{2022, 1, 1}).value()};

    QTest::addRow("Ascending sort")
        << DateAddedSort{} << MediaGenerator{[firstMedium, secondMedium, thirdMedium] {
               std::vector<std::unique_ptr<Medium>> media;
               media.push_back(std::make_unique<Article>(secondMedium));
               media.push_back(std::make_unique<Video>(thirdMedium));
               media.push_back(std::make_unique<Book>(firstMedium));
               return media;
           }}
        << std::vector{firstMedium.id(), secondMedium.id(), thirdMedium.id()};

    QTest::addRow("descending sort")
        << DateAddedSort{false} << MediaGenerator{[firstMedium, secondMedium, thirdMedium]() {
               std::vector<std::unique_ptr<Medium>> media;
               media.push_back(std::make_unique<Article>(secondMedium));
               media.push_back(std::make_unique<Video>(thirdMedium));
               media.push_back(std::make_unique<Book>(firstMedium));
               return media;
           }}
        << std::vector{thirdMedium.id(), secondMedium.id(), firstMedium.id()};
}
void TestDateAddedSort::testApply() {
    QFETCH(DateAddedSort, sort);
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(std::vector<QUuid>, expectedIdsSorting);

    std::vector media{mediaGenerator()};
    auto mediaPtrsView{media | std::views::transform([](const auto &m) { return m.get(); })};
    std::vector<const Medium *> mediaPtrs{mediaPtrsView.begin(), mediaPtrsView.end()};

    std::vector sortedMedia{sort.apply(std::move(mediaPtrs))};

    auto sortedMediaIdsView{sortedMedia |
                            std::views::transform([](const Medium *mp) { return mp->id(); })};
    std::vector<QUuid> sortedMediaIds{sortedMediaIdsView.begin(), sortedMediaIdsView.end()};

    QCOMPARE(sortedMediaIds, expectedIdsSorting);
}