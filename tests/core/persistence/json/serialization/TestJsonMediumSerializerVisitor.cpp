#include "TestJsonMediumSerializerVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/serialization/JsonMediumSerializerVisitor.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QJsonObject>
#include <QTest>
#include <type_traits>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::MediumUserData;
using Core::Model::Video;
using Core::Persistence::MediaSerializationConfigs;
using Core::Persistence::Json::JsonMediumSerializerVisitor;

void TestJsonMediumSerializerVisitor::testVisitBook_data() {
    QTest::addColumn<Book>("book");
    QTest::addColumn<QJsonObject>("expectedSerialization");

    MediaSerializationConfigs defaultConfigs{};

    const QString title{"Thinking, Fast and Slow"};
    const Book minimalBook{Book::create(title).value()};
    const QJsonObject minimalBookJson{
        {"type", "book"},
        {"id", minimalBook.id().toString(QUuid::StringFormat::WithoutBraces)},
        {"title", title},
        {"favorite", false},
    };

    QTest::addRow("Book - only base attributes (id, title, favorite)")
        << minimalBook << minimalBookJson;

    BookJsonFixture fixture;

    QTest::addRow("Book - full book with Internet url") << fixture.book << fixture.json;

    Book fullBookLocalThumbnail{fixture.book};
    fullBookLocalThumbnail.thumbnailUrl().set(
        QUrl::fromLocalFile("/Users/sourcevaultuser/image.png"));
    QJsonObject fullBookLocalThumbnailJson{fixture.json};
    fullBookLocalThumbnailJson.remove("thumbnailUrl");
    QTest::addRow("Book - full book with local url: url shouldn't be exported")
        << fullBookLocalThumbnail << fullBookLocalThumbnailJson;
}
void TestJsonMediumSerializerVisitor::testVisitBook() {
    QFETCH(Book, book);
    QFETCH(QJsonObject, expectedSerialization);

    JsonMediumSerializerVisitor visitor;

    book.accept(visitor);

    const auto result{visitor.getSerializedMedium()};
    QCOMPARE(result, expectedSerialization);
}

void TestJsonMediumSerializerVisitor::testVisitArticle_data() {
    QTest::addColumn<Article>("article");
    QTest::addColumn<QJsonObject>("expectedSerialization");

    MediaSerializationConfigs defaultConfigs{};

    const QString title{"How We Traded Anxiety for Apathy"};
    const Article minimalArticle{Article::create(title).value()};
    const QJsonObject minimalArticleJson{
        {"type", "article"},
        {"id", minimalArticle.id().toString(QUuid::StringFormat::WithoutBraces)},
        {"title", title},
        {"favorite", false},
    };

    QTest::addRow("Article - only base attributes (id, title, favorite)")
        << minimalArticle << minimalArticleJson;

    ArticleJsonFixture fixture;
    QTest::addRow("Article - full") << fixture.article << fixture.json;
}
void TestJsonMediumSerializerVisitor::testVisitArticle() {
    QFETCH(Article, article);
    QFETCH(QJsonObject, expectedSerialization);

    JsonMediumSerializerVisitor visitor;

    article.accept(visitor);

    const auto result{visitor.getSerializedMedium()};
    QCOMPARE(result, expectedSerialization);
}

void TestJsonMediumSerializerVisitor::testVisitVideo_data() {
    QTest::addColumn<Video>("video");
    QTest::addColumn<QJsonObject>("expectedSerialization");

    MediaSerializationConfigs defaultConfigs{};

    const QString title{"Educati e Finanziati 01: introduzione al nuovo corso di finanza di base"};
    const Video minimalVideo{Video::create(title).value()};
    const QJsonObject minimalVideoJson{
        {"type", "video"},
        {"id", minimalVideo.id().toString(QUuid::StringFormat::WithoutBraces)},
        {"title", title},
        {"favorite", false},
    };

    QTest::addRow("Video - only base attributes (id, title, favorite)")
        << minimalVideo << minimalVideoJson;

    VideoJsonFixture fixture;
    QTest::addRow("Video - full video with Internet url") << fixture.video << fixture.json;

    Video fullVideoLocalThumbnail{fixture.video};
    fullVideoLocalThumbnail.thumbnailUrl().set(
        QUrl::fromLocalFile("/Users/sourcevaultuser/image.png"));
    QJsonObject fullBookLocalThumbnailJson{fixture.json};
    fullBookLocalThumbnailJson.remove("thumbnailUrl");
    QTest::addRow("Video - full video with local url: url shouldn't be exported")
        << fullVideoLocalThumbnail << fullBookLocalThumbnailJson;
}
void TestJsonMediumSerializerVisitor::testVisitVideo() {
    QFETCH(Video, video);
    QFETCH(QJsonObject, expectedSerialization);

    JsonMediumSerializerVisitor visitor;

    video.accept(visitor);

    const auto result{visitor.getSerializedMedium()};
    QCOMPARE(result, expectedSerialization);
}