#include "TestMediumTypeFilter.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "queries/filters/MediumTypeFilter.h"

#include <QTest>
#include <algorithm>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Queries::Filters::MediumTypeFilter;

using MediaGenerator = std::function<std::vector<std::unique_ptr<Medium>>()>;

void TestMediumTypeFilter::testClone() {
    const auto filter{
        std::make_unique<MediumTypeFilter>(std::unordered_set<std::type_index>{typeid(Video)})};

    const auto clone{filter->clone()};

    QVERIFY(filter != clone);
    QVERIFY(dynamic_cast<const MediumTypeFilter *>(clone.get()) != nullptr);

    QVERIFY(clone->matches(std::make_unique<Video>(Video::create("video").value()).get()));
    QVERIFY(!clone->matches(std::make_unique<Book>(Book::create("book").value()).get()));
    QVERIFY(!clone->matches(std::make_unique<Article>(Article::create("article").value()).get()));
}

void TestMediumTypeFilter::testMatches_data() {
    QTest::addColumn<MediumTypeFilter>("filter");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<std::set<QUuid>>("expectedMatchingIds");

    const Book book{Book::create("book").value()};
    const Article article{Article::create("article").value()};
    const Video video{Video::create("video").value()};
    MediaGenerator generator{[book, article, video] {
        std::vector<std::unique_ptr<Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        media.push_back(std::make_unique<Article>(article));
        media.push_back(std::make_unique<Video>(video));
        return media;
    }};

    QTest::addRow("Empty filters don't match anything")
        << MediumTypeFilter{{}} << generator << std::set<QUuid>{};
    QTest::addRow("Single type filter")
        << MediumTypeFilter{{typeid(Book)}} << generator << std::set{book.id()};
    QTest::addRow("Two types filter") << MediumTypeFilter{{typeid(Book), typeid(Article)}}
                                      << generator << std::set{book.id(), article.id()};
    QTest::addRow("Full filter") << MediumTypeFilter{{typeid(Book), typeid(Article), typeid(Video)}}
                                 << generator << std::set{book.id(), article.id(), video.id()};
}
void TestMediumTypeFilter::testMatches() {
    QFETCH(MediumTypeFilter, filter);
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(std::set<QUuid>, expectedMatchingIds);

    std::vector<std::unique_ptr<Medium>> media{mediaGenerator()};

    auto matchedMediaView{
        media | std::views::transform([](const auto &m) { return m.get(); }) |
        std::views::filter([&filter](const auto m) { return filter.matches(m); })};
    const std::vector<Medium *> matchedMediaVector{matchedMediaView.begin(),
                                                   matchedMediaView.end()};

    QCOMPARE(matchedMediaVector.size(), expectedMatchingIds.size());

    std::ranges::for_each(matchedMediaVector, [&expectedMatchingIds](const auto m) {
        QVERIFY(expectedMatchingIds.contains(m->id()));
    });
}