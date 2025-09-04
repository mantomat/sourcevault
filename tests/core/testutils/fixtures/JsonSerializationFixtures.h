#ifndef JSONSERIALIZATIONFIXTURES_H
#define JSONSERIALIZATIONFIXTURES_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/MediumUserData.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "testutils/mocks/MockConcreteMedium.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Medium;
using Core::Model::MediumUserData;
using Core::Model::Video;
using Core::Persistence::MediaSerializationConfigs;

struct ArticleJsonFixture {
    Article article{[] {
        Article newArticle =
            Article::create("Test Title", QUuid{"{9c1f1ba8-a826-45fa-bdfb-b53743384220}"},
                            QDate(2025, 1, 1))
                .value();
        newArticle.userData().favorite() = true;
        newArticle.authors().set({"Joan Westenberg"});
        newArticle.language().set("English");
        newArticle.userData().topics().set({"Emotions", "Personal Growth"});
        newArticle.userData().notes().set("Example notes...");
        newArticle.userData().priority().set(MediumUserData::PriorityLevel::mid);
        newArticle.articleUrl().set(QUrl{
            "https://www.joanwestenberg.com/p/how-we-traded-anxiety-for-apathy-d9fec4ba127f6971"});
        newArticle.sourceName().set("Westenber Blog");
        newArticle.readTimeMinutes().set(5);
        newArticle.publicationDate().set(QDate{2025, 8, 11});
        return newArticle;
    }()};

    QJsonObject json{
        {"type", "article"},
        {"id", article.id().toString(QUuid::WithoutBraces)},
        {"title", article.title()},
        {"favorite", article.userData().favorite()},
        {"authors", QJsonArray{"Joan Westenberg"}},
        {"language", article.language().get().value()},
        {"topics", QJsonArray{"Emotions", "Personal Growth"}},
        {"notes", article.userData().notes().get().value()},
        {"priority", static_cast<int>(article.userData().priority().get().value())},
        {"articleUrl", article.articleUrl().get().value().toString()},
        {"sourceName", article.sourceName().get().value()},
        {"readTimeMinutes", static_cast<int>(article.readTimeMinutes().get().value())},
        {"publicationDate",
         article.publicationDate().get()->toString(MediaSerializationConfigs{}.dateFormat)},
    };
};

struct VideoJsonFixture {
    Video video{[] {
        Video newVideo{Video::create("Test Title", QUuid{"{88000708-d23e-498e-afac-4d956bc5d030}"},
                                     QDate(2025, 1, 1))
                           .value()};
        newVideo.userData().favorite() = false;
        newVideo.authors().set({"Paolo Coletti"});
        newVideo.language().set("Italian");
        newVideo.userData().topics().set({"Education", "Finance"});
        newVideo.userData().notes().set("Example notes...");
        newVideo.userData().priority().set(MediumUserData::PriorityLevel::max);
        newVideo.videoUrl().set(QUrl{
            "https://www.youtube.com/watch?v=cSlr2I1GGiw&list=PLfuzpc-H8qcdyEb5rpgYhN2P7cJGR1i9h"});
        newVideo.durationSeconds().set(516);
        newVideo.uploadDate().set(QDate{2023, 8, 13});
        newVideo.thumbnailUrl().set(
            QUrl{"https://i.ytimg.com/vi/cSlr2I1GGiw/"
                 "hqdefault.jpg?sqp=-oaymwEcCPYBEIoBSFXyq4qpAw4IARUAAIhCGAFwAcABBg==&rs="
                 "AOn4CLAyPvvwnV5Kt15d_r8EvE72ZEJHfg"});
        return newVideo;
    }()};

    QJsonObject json{
        {"type", "video"},
        {"id", video.id().toString(QUuid::WithoutBraces)},
        {"title", video.title()},
        {"favorite", video.userData().favorite()},
        {"authors", QJsonArray{"Paolo Coletti"}},
        {"language", video.language().get().value()},
        {"topics", QJsonArray{"Education", "Finance"}},
        {"notes", video.userData().notes().get().value()},
        {"priority", static_cast<int>(video.userData().priority().get().value())},
        {"videoUrl", video.videoUrl().get().value().toString()},
        {"durationSeconds", static_cast<int>(video.durationSeconds().get().value())},
        {"uploadDate", video.uploadDate().get().value().toString(
                           Core::Persistence::MediaSerializationConfigs{}.dateFormat)},
        {"thumbnailUrl", video.thumbnailUrl().get().value().toString()},
    };
};

struct BookJsonFixture {
    Book book{[] {
        Book newBook{Book::create("Test Title", QUuid{"{fc1142b9-3d6d-44b0-aa1a-36f8bbbf22d8}"},
                                  QDate(2025, 1, 1))
                         .value()};
        newBook.userData().favorite() = true;
        newBook.authors().set({"Daniel Kahneman"});
        newBook.language().set("English");
        newBook.userData().topics().set({"Economics", "Psychology"});
        newBook.userData().notes().set("Example notes...");
        newBook.userData().priority().set(MediumUserData::PriorityLevel::high);
        newBook.isbn().set("978-0141033570");
        newBook.edition().set("First edition");
        newBook.publisher().set("Penguin Psychology");
        newBook.yearPublished().set(2012);
        newBook.pageNumber().set(610);
        newBook.description().set("One of the most influential books of the 21st century: ...");
        newBook.thumbnailUrl().set(
            QUrl{"https://m.media-amazon.com/images/I/71f6DceqZAL._SL1500_.jpg"});
        return newBook;
    }()};

    QJsonObject json{
        {"type", "book"},
        {"id", book.id().toString(QUuid::WithoutBraces)},
        {"title", book.title()},
        {"favorite", book.userData().favorite()},
        {"authors", QJsonArray{"Daniel Kahneman"}},
        {"language", book.language().get().value()},
        {"topics", QJsonArray{"Economics", "Psychology"}},
        {"notes", book.userData().notes().get().value()},
        {"priority", static_cast<int>(book.userData().priority().get().value())},
        {"isbn", book.isbn().get().value()},
        {"edition", book.edition().get().value()},
        {"publisher", book.publisher().get().value()},
        {"yearPublished", book.yearPublished().get().value()},
        {"pageNumber", static_cast<int>(book.pageNumber().get().value())},
        {"description", book.description().get().value()},
        {"thumbnailUrl", book.thumbnailUrl().get().value().toString()},
    };
};

struct MediumJsonFixture {
    MockConcreteMedium medium{[] {
        MockConcreteMedium newMedium;
        newMedium.userData().favorite() = true;
        newMedium.authors().set({"Joan Westenberg"});
        newMedium.language().set("English");
        newMedium.userData().topics().set({"Emotions", "Personal Growth"});
        newMedium.userData().notes().set("Example notes...");
        newMedium.userData().priority().set(MediumUserData::PriorityLevel::mid);
        return newMedium;
    }()};

    QJsonObject json{
        {"type", "unused"},
        {"id", medium.id().toString(QUuid::WithoutBraces)},
        {"title", medium.title()},
        {"favorite", medium.userData().favorite()},
        {"authors", QJsonArray{"Joan Westenberg"}},
        {"language", medium.language().get().value()},
        {"topics", QJsonArray{"Emotions", "Personal Growth"}},
        {"notes", medium.userData().notes().get().value()},
        {"priority", static_cast<int>(medium.userData().priority().get().value())},
    };
};

struct JsonLibraryFixture {
    Library library{[] {
        Library newLibrary{};
        newLibrary.addMedium(std::make_unique<Article>(ArticleJsonFixture{}.article));
        newLibrary.addMedium(std::make_unique<Book>(BookJsonFixture{}.book));
        newLibrary.addMedium(std::make_unique<Video>(VideoJsonFixture{}.video));
        return newLibrary;
    }()};

    QJsonDocument json{
        QJsonObject{{"version", "1.0"},
                    {"media", QJsonArray{ArticleJsonFixture{}.json, BookJsonFixture{}.json,
                                         VideoJsonFixture{}.json}}}};
};

#endif