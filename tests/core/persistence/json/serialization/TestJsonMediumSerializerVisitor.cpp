#include "TestJsonMediumSerializerVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "persistence/json/serialization/JsonMediumSerializerVisitor.h"

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

    const bool favorite{true};
    const std::set<QString> authors{"Daniel Kahneman"};
    const QString language{"English"};
    const std::set<QString> topics{"Psychology", "Economics"};
    const QString notes{"Example notes..."};
    const auto priority{MediumUserData::PriorityLevel::high};
    const QString isbn{"978-0141033570"};
    const QString edition{"First edition"};
    const QString publisher{"Penguin Psychology"};
    constexpr int yearPublished{2012};
    constexpr unsigned int pageNumber{610};
    const QString description{"One of the most influential books of the 21st century: ..."};
    const QUrl thumbnailUrl{"https://m.media-amazon.com/images/I/71f6DceqZAL._SL1500_.jpg"};

    Book fullBook{minimalBook};
    fullBook.userData().favorite() = favorite;
    fullBook.authors().set(authors);
    fullBook.language().set(language);
    fullBook.userData().topics().set(topics);
    fullBook.userData().notes().set(notes);
    fullBook.userData().priority().set(priority);
    fullBook.isbn().set(isbn);
    fullBook.edition().set(edition);
    fullBook.publisher().set(publisher);
    fullBook.yearPublished().set(yearPublished);
    fullBook.pageNumber().set(pageNumber);
    fullBook.description().set(description);

    QJsonObject fullBookJson{minimalBookJson};
    fullBookJson["favorite"] = favorite;
    fullBookJson["authors"] = QJsonArray::fromStringList({authors.begin(), authors.end()});
    fullBookJson["language"] = language;
    fullBookJson["topics"] = QJsonArray::fromStringList({topics.begin(), topics.end()});
    fullBookJson["notes"] = notes;
    fullBookJson["priority"] = std::underlying_type_t<MediumUserData::PriorityLevel>(priority);
    fullBookJson["isbn"] = isbn;
    fullBookJson["edition"] = edition;
    fullBookJson["publisher"] = publisher;
    fullBookJson["yearPublished"] = yearPublished;
    fullBookJson["pageNumber"] = static_cast<int>(pageNumber);
    fullBookJson["description"] = description;

    QTest::addRow("Book - full book with Internet url") << fullBook << fullBookJson;

    Book fullBookLocalThumbnail{fullBook};
    fullBookLocalThumbnail.thumbnailUrl().set(
        QUrl::fromLocalFile("/Users/sourcevaultuser/image.png"));
    QJsonObject fullBookLocalThumbnailJson{fullBookJson};
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

    const bool favorite{true};
    const std::set<QString> authors{"Joan Westenberg"};
    const QString language{"English"};
    const std::set<QString> topics{"Emotions", "Personal Growth"};
    const QString notes{"Example notes..."};
    const auto priority{MediumUserData::PriorityLevel::mid};
    const QUrl articleUrl{
        "https://www.joanwestenberg.com/p/how-we-traded-anxiety-for-apathy-d9fec4ba127f6971"};
    const QString sourceName{"Westenberg Blog"};
    const unsigned int readTimeMinutes{5};
    const QDate publicationDate{2025, 8, 11};

    Article fullArticle{minimalArticle};
    fullArticle.userData().favorite() = favorite;
    fullArticle.authors().set(authors);
    fullArticle.language().set(language);
    fullArticle.userData().topics().set(topics);
    fullArticle.userData().notes().set(notes);
    fullArticle.userData().priority().set(priority);
    fullArticle.articleUrl().set(articleUrl);
    fullArticle.sourceName().set(sourceName);
    fullArticle.readTimeMinutes().set(readTimeMinutes);
    fullArticle.publicationDate().set(publicationDate);

    QJsonObject fullArticleJson{minimalArticleJson};
    fullArticleJson["favorite"] = favorite;
    fullArticleJson["authors"] = QJsonArray::fromStringList({authors.begin(), authors.end()});
    fullArticleJson["language"] = language;
    fullArticleJson["topics"] = QJsonArray::fromStringList({topics.begin(), topics.end()});
    fullArticleJson["notes"] = notes;
    fullArticleJson["priority"] = std::underlying_type_t<MediumUserData::PriorityLevel>(priority);
    fullArticleJson["articleUrl"] = articleUrl.toString();
    fullArticleJson["sourceName"] = sourceName;
    fullArticleJson["readTimeMinutes"] = static_cast<int>(readTimeMinutes);
    fullArticleJson["publicationDate"] = publicationDate.toString(defaultConfigs.dateFormat);

    QTest::addRow("Article - full") << fullArticle << fullArticleJson;
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

    const bool favorite{true};
    const std::set<QString> authors{"Paolo Coletti"};
    const QString language{"Italian"};
    const std::set<QString> topics{"Finance", "Education"};
    const QString notes{"Thank you prof!"};
    const auto priority{MediumUserData::PriorityLevel::max};
    const QUrl videoUrl{
        "https://www.youtube.com/watch?v=cSlr2I1GGiw&list=PLfuzpc-H8qcdyEb5rpgYhN2P7cJGR1i9h"};
    const unsigned int durationSeconds{516};
    const QDate uploadDate{2023, 8, 13};
    const QUrl thumbnailUrl{"https://i.ytimg.com/vi/cSlr2I1GGiw/"
                            "hqdefault.jpg?sqp=-oaymwEcCPYBEIoBSFXyq4qpAw4IARUAAIhCGAFwAcABBg==&rs="
                            "AOn4CLAyPvvwnV5Kt15d_r8EvE72ZEJHfg"};

    Video fullVideo{minimalVideo};
    fullVideo.userData().favorite() = favorite;
    fullVideo.authors().set(authors);
    fullVideo.language().set(language);
    fullVideo.userData().topics().set(topics);
    fullVideo.userData().notes().set(notes);
    fullVideo.userData().priority().set(priority);
    fullVideo.videoUrl().set(videoUrl);
    fullVideo.durationSeconds().set(durationSeconds);
    fullVideo.uploadDate().set(uploadDate);
    fullVideo.thumbnailUrl().set(thumbnailUrl);

    QJsonObject fullVideoJson{minimalVideoJson};
    fullVideoJson["favorite"] = favorite;
    fullVideoJson["authors"] = QJsonArray::fromStringList({authors.begin(), authors.end()});
    fullVideoJson["language"] = language;
    fullVideoJson["topics"] = QJsonArray::fromStringList({topics.begin(), topics.end()});
    fullVideoJson["notes"] = notes;
    fullVideoJson["priority"] = std::underlying_type_t<MediumUserData::PriorityLevel>(priority);
    fullVideoJson["videoUrl"] = videoUrl.toString();
    fullVideoJson["durationSeconds"] = static_cast<int>(durationSeconds);
    fullVideoJson["uploadDate"] = uploadDate.toString(defaultConfigs.dateFormat);
    fullVideoJson["thumbnailUrl"] = thumbnailUrl.toString();

    QTest::addRow("Video - full video with Internet url") << fullVideo << fullVideoJson;

    Video fullVideoLocalThumbnail{fullVideo};
    fullVideoLocalThumbnail.thumbnailUrl().set(
        QUrl::fromLocalFile("/Users/sourcevaultuser/image.png"));
    QJsonObject fullBookLocalThumbnailJson{fullVideoJson};
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