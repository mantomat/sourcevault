#include "TestJsonMediaSerializer.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/Video.h"
#include "persistence/json/serialization/JsonMediaSerializer.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::Video;
using Core::Persistence::Json::JsonMediaSerializer;

using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;

void TestJsonMediaSerializer::testSerialize_data() {
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<QJsonDocument>("expectedJsonDocument");

    Book book{Book::create("title").value()};
    Article article{Article::create("title").value()};
    Video video{Video::create("title").value()};

    const QString expectedVersion{JsonMediaSerializer::version};
    QTest::addRow("Empty library serialization")
        << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }}
        << QJsonDocument{QJsonObject{{"media", QJsonArray{}}, {"version", expectedVersion}}};

    QJsonDocument singleMediaDocument{QJsonObject{
        {"version", expectedVersion},
        {"media",
         QJsonArray{{QJsonObject{{"type", "book"},
                                 {"id", book.id().toString(QUuid::StringFormat::WithoutBraces)},
                                 {"title", book.title()},
                                 {"favorite", book.userData().favorite()}}}}}}};
    QTest::addRow("Single media library serialization") << MediaGenerator{[book] {
        std::vector<std::unique_ptr<const Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        return media;
    }} << singleMediaDocument;

    const QJsonDocument multipleMediaDocument{QJsonObject{
        {"version", expectedVersion},
        {"media",
         QJsonArray{{QJsonObject{{"type", "book"},
                                 {"id", book.id().toString(QUuid::StringFormat::WithoutBraces)},
                                 {"title", book.title()},
                                 {"favorite", book.userData().favorite()}},
                     {QJsonObject{{"type", "article"},
                                  {"id", article.id().toString(QUuid::StringFormat::WithoutBraces)},
                                  {"title", article.title()},
                                  {"favorite", article.userData().favorite()}}},
                     {QJsonObject{{"type", "video"},
                                  {"id", video.id().toString(QUuid::StringFormat::WithoutBraces)},
                                  {"title", video.title()},
                                  {"favorite", video.userData().favorite()}}}}}}}};
    QTest::addRow("Multiple media library serialization") << MediaGenerator{[book, article, video] {
        std::vector<std::unique_ptr<const Medium>> media;
        media.push_back(std::make_unique<Book>(book));
        media.push_back(std::make_unique<Article>(article));
        media.push_back(std::make_unique<Video>(video));
        return media;
    }} << multipleMediaDocument;
}
void TestJsonMediaSerializer::testSerialize() {
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(QJsonDocument, expectedJsonDocument);

    auto media{mediaGenerator()};
    auto mediaPtrsView{media |
                       std::views::transform([](const auto &medium) { return medium.get(); })};
    std::vector<const Medium *> mediaPtrs{mediaPtrsView.begin(), mediaPtrsView.end()};

    JsonMediaSerializer serializer;

    QCOMPARE(serializer.serialize(mediaPtrs), expectedJsonDocument);
}