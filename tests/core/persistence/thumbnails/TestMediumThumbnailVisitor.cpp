#include "TestMediumThumbnailVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "persistence/thumbnails/MediumThumbnailVisitor.h"

#include <QTest>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Persistence::Thumbnails::MediumThumbnailVisitor;

void TestMediumThumbnailVisitor::testVisitArticle_data() {
    QTest::addColumn<Article>("article");

    Article basicArticle{Article::create("test").value()};

    QTest::addRow("No thumbnail article - an Article doesn't have a thumbnail, hence it never "
                  "returns its location")
        << basicArticle;
}
void TestMediumThumbnailVisitor::testVisitArticle() {
    QFETCH(Article, article);

    MediumThumbnailVisitor visitor;

    article.accept(visitor);
    QCOMPARE(visitor.getLocalThumbnailPath(), std::optional<QString>{std::nullopt});
}

void TestMediumThumbnailVisitor::testVisitBook_data() const {
    QTest::addColumn<Book>("book");
    QTest::addColumn<std::optional<QString>>("expectedThumbnailLocation");

    Book basicBook{Book::create("test").value()};

    QTest::addRow("A book without thumbnail does not return the location")
        << basicBook << std::optional<QString>{std::nullopt};
    {
        Book withInvalidThumbnailLocation{basicBook};
        QString nonExistentPath{tempDir.filePath("non-existent-file.jpg")};
        bool didSet{
            withInvalidThumbnailLocation.thumbnailUrl().set(QUrl::fromLocalFile(nonExistentPath))};
        assert(didSet);
        QTest::addRow(
            "Book has thumbnail url but it points to an invalid location -> no location returned")
            << withInvalidThumbnailLocation << std::optional<QString>{std::nullopt};
    }
    {
        Book withValidThumbnail{basicBook};
        QString thumbnailPath{tempDir.filePath("book-thumbnail.jpg")};
        const bool didSet{
            withValidThumbnail.thumbnailUrl().set(QUrl::fromLocalFile(thumbnailPath))};
        assert(didSet);

        QFile{thumbnailPath}.open(QFileDevice::WriteOnly);

        QTest::addRow(
            "Book has thumbnail url and it points to a valid location -> location is returned")
            << withValidThumbnail
            << std::make_optional<QString>(
                   withValidThumbnail.thumbnailUrl().get().value().toLocalFile());
    }
}
void TestMediumThumbnailVisitor::testVisitBook() {
    QFETCH(Book, book);
    QFETCH(std::optional<QString>, expectedThumbnailLocation);

    MediumThumbnailVisitor visitor;

    book.accept(visitor);
    QCOMPARE(visitor.getLocalThumbnailPath(),
             expectedThumbnailLocation.has_value()
                 ? std::make_optional<QString>(expectedThumbnailLocation.value())
                 : std::nullopt);
}

void TestMediumThumbnailVisitor::testVisitVideo_data() const {
    QTest::addColumn<Video>("video");
    QTest::addColumn<std::optional<QString>>("expectedThumbnailLocation");

    const Video basicVideo{Video::create("test").value()};

    QTest::addRow("A video without thumbnail does not return the location")
        << basicVideo << std::optional<QString>{std::nullopt};

    {
        Video withInvalidThumbnailLocation{basicVideo};
        QString nonExistentPath{tempDir.filePath("non-existent-file.jpg")};
        bool didSet{
            withInvalidThumbnailLocation.thumbnailUrl().set(QUrl::fromLocalFile(nonExistentPath))};
        assert(didSet);
        QTest::addRow(
            "Video has thumbnail url but it points to an invalid location -> no location returned")
            << withInvalidThumbnailLocation << std::optional<QString>{std::nullopt};
    }
    {
        Video withValidThumbnail{basicVideo};
        QString thumbnailPath{tempDir.filePath("video-thumbnail.jpg")};
        const bool didSet{
            withValidThumbnail.thumbnailUrl().set(QUrl::fromLocalFile(thumbnailPath))};
        assert(didSet);

        QFile{thumbnailPath}.open(QFileDevice::WriteOnly);

        QTest::addRow(
            "Video has thumbnail url and it points to a valid location -> location is returned")
            << withValidThumbnail
            << std::make_optional<QString>(
                   withValidThumbnail.thumbnailUrl().get().value().toLocalFile());
    }
}
void TestMediumThumbnailVisitor::testVisitVideo() {
    QFETCH(Video, video);
    QFETCH(std::optional<QString>, expectedThumbnailLocation);

    MediumThumbnailVisitor visitor;

    video.accept(visitor);
    QCOMPARE(visitor.getLocalThumbnailPath(),
             expectedThumbnailLocation.has_value()
                 ? std::make_optional<QString>(expectedThumbnailLocation.value())
                 : std::nullopt);
}