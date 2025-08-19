#include "TestMediumTypeVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/Video.h"
#include "shared/MediumTypeVisitor.h"

#include <QTest>
#include <ranges>
#include <unordered_set>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::Video;

using Core::Shared::MediumTypeVisitor;

using MediaGenerator = std::function<std::vector<std::unique_ptr<Medium>>()>;

void TestMediumTypeVisitor::testHasMatched_data() {

    QTest::addColumn<MediumTypeVisitor>("visitor");
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<std::set<QUuid>>("expectedMatchIds");

    const auto book{Book::create("title").value()};
    const auto video{Video::create("title").value()};
    const auto article{Article::create("title").value()};

    const MediaGenerator mediaGenerator{[book, video, article] {
        std::vector<std::unique_ptr<Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        media.push_back(std::make_unique<Video>(video));
        media.push_back(std::make_unique<Article>(article));
        return media;
    }};

    QTest::addRow("Match single medium type")
        << MediumTypeVisitor{{typeid(Book)}} << mediaGenerator << std::set{book.id()};

    QTest::addRow("Match two media type") << MediumTypeVisitor{{typeid(Book), typeid(Article)}}
                                          << mediaGenerator << std::set{book.id(), article.id()};

    QTest::addRow("Match three media type")
        << MediumTypeVisitor{{typeid(Book), typeid(Article), typeid(Video)}} << mediaGenerator
        << std::set{book.id(), article.id(), video.id()};
}
void TestMediumTypeVisitor::testHasMatched() {
    QFETCH(MediumTypeVisitor, visitor);
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(std::set<QUuid>, expectedMatchIds);

    const auto media{mediaGenerator()};

    auto matchedMediaIdsView{media | std::views::filter([&visitor](const auto &m) {
                                 m->accept(visitor);
                                 return visitor.hasMatched();
                             }) |
                             std::views::transform([](const auto &m) { return m->id(); })};
    const std::set<QUuid> matchedMediaIds{matchedMediaIdsView.begin(), matchedMediaIdsView.end()};

    QCOMPARE(matchedMediaIds, expectedMatchIds);
}

void TestMediumTypeVisitor::testVisitBook() {
    auto book{Book::create("title").value()};

    book.accept(bookVisitor);
    QCOMPARE(bookVisitor.hasMatched(), true);

    book.accept(articleVisitor);
    QCOMPARE(articleVisitor.hasMatched(), false);

    book.accept(videoVisitor);
    QCOMPARE(videoVisitor.hasMatched(), false);
}

void TestMediumTypeVisitor::testVisitArticle() {
    auto article{Article::create("title").value()};

    article.accept(bookVisitor);
    QCOMPARE(bookVisitor.hasMatched(), false);

    article.accept(articleVisitor);
    QCOMPARE(articleVisitor.hasMatched(), true);

    article.accept(videoVisitor);
    QCOMPARE(videoVisitor.hasMatched(), false);
}

void TestMediumTypeVisitor::testVisitVideo() {
    auto video{Video::create("title").value()};

    video.accept(bookVisitor);
    QCOMPARE(bookVisitor.hasMatched(), false);

    video.accept(articleVisitor);
    QCOMPARE(articleVisitor.hasMatched(), false);

    video.accept(videoVisitor);
    QCOMPARE(videoVisitor.hasMatched(), true);
}