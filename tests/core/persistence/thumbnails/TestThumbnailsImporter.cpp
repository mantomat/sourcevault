#include "TestThumbnailsImporter.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "persistence/json/deserialization/TestJsonMediaDeserializer.h"
#include "persistence/thumbnails/ThumbnailsImporter.h"
#include "shared/OverloadedVariantVisitor.h"

#include <QDir>
#include <QTest>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Persistence::Thumbnails::ThumbnailsImporter;
using Core::Shared::OverloadedVariantVisitor;

namespace {

const char *const thumbnailExtension{"jpg"};

}

void TestThumbnailsImporter::testImportLocalThumbnails_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<QString>("thumbnailsDirPath");
    QTest::addColumn<ImportResult>("expectedResult");

    QTest::addRow("Invalid thumbnails directory -> FileError")
        << Library{} << tempDir.filePath("non-existent")
        << ImportResult{QFileDevice::PositionError};

    {
        QDir thumbnailsDir{tempDir.path()};
        QString dirName{"no-thumbnails"};
        assert(thumbnailsDir.mkdir(dirName));
        assert(thumbnailsDir.cd(dirName));

        QFile{thumbnailsDir.filePath("unrelated-file.txt")}.open(QFile::WriteOnly);

        Library lib;
        lib.addMedium(Book::make("title").value());
        lib.addMedium(Video::make("title").value());
        lib.addMedium(Article::make("title").value());

        QTest::addRow(
            "Non-empty library, valid thumbnails directory with other files -> no imports")
            << lib << thumbnailsDir.path()
            << ImportResult{std::make_pair(lib, std::unordered_set<QUuid>{})};
    }

    {
        QDir thumbnailsDir{tempDir.path()};
        QString dirName{"unrelated-thumbnails"};
        assert(thumbnailsDir.mkdir(dirName));
        assert(thumbnailsDir.cd(dirName));

        QString unrelatedId{QUuid::createUuid().toString(QUuid::WithoutBraces)};
        QFile{thumbnailsDir.filePath(unrelatedId + "." + thumbnailExtension)}.open(
            QFile::WriteOnly);

        Library lib;
        lib.addMedium(Book::make("title").value());
        lib.addMedium(Video::make("title").value());
        lib.addMedium(Article::make("title").value());

        QTest::addRow("Non-empty library, valid thumbnails directory with well-named files but no "
                      "corresponding media in library -> no imports")
            << lib << thumbnailsDir.path()
            << ImportResult{std::make_pair(lib, std::unordered_set<QUuid>{})};
    }

    {
        QDir thumbnailsDir{tempDir.path()};
        QString dirName{"valid-thumbnails"};
        assert(thumbnailsDir.mkdir(dirName));
        assert(thumbnailsDir.cd(dirName));

        auto article{Article::create("title").value()};
        auto video{Video::create("title").value()};
        auto book{Book::create("title").value()};

        const QString articleId{article.id().toString(QUuid::WithoutBraces)};
        QFile{thumbnailsDir.filePath(articleId + "." + thumbnailExtension)}.open(QFile::WriteOnly);

        const QString videoId{video.id().toString(QUuid::WithoutBraces)};
        const QString videoThumbnailFilename{videoId + "." + thumbnailExtension};
        QFile{thumbnailsDir.filePath(videoThumbnailFilename)}.open(QFile::WriteOnly);

        const QString bookId{book.id().toString(QUuid::WithoutBraces)};
        const QString bookThumbnailFilename{bookId + "." + thumbnailExtension};
        QFile{thumbnailsDir.filePath(bookThumbnailFilename)}.open(QFile::WriteOnly);

        Library lib;
        lib.addMedium(std::make_unique<Article>(article));
        lib.addMedium(std::make_unique<Video>(video));
        lib.addMedium(std::make_unique<Book>(book));

        auto expectedVideo{video};
        assert(expectedVideo.thumbnailUrl().set(
            QUrl::fromLocalFile(thumbnailsDir.filePath(videoThumbnailFilename))));

        auto expectedBook{book};
        assert(expectedBook.thumbnailUrl().set(
            QUrl::fromLocalFile(thumbnailsDir.filePath(bookThumbnailFilename))));

        Library expectedLib;
        expectedLib.addMedium(std::make_unique<Article>(article));
        expectedLib.addMedium(std::make_unique<Video>(expectedVideo));
        expectedLib.addMedium(std::make_unique<Book>(expectedBook));

        QTest::addRow(
            "Non-empty library, valid thumbnails directory with well-named files with "
            "corresponding media in library -> correctly imports (no article import obviously)")
            << lib << thumbnailsDir.path()
            << ImportResult{std::make_pair(expectedLib, std::unordered_set{book.id(), video.id()})};
    }
}
void TestThumbnailsImporter::testImportLocalThumbnails() {

    QFETCH(Library, library);
    QFETCH(QString, thumbnailsDirPath);
    QFETCH(ImportResult, expectedResult);

    const ThumbnailsImporter importer;

    const ImportResult result{importer.importLocalThumbnails(library, thumbnailsDirPath)};

    std::visit(OverloadedVariantVisitor{
                   [&result](const QFileDevice::FileError &err) { testImportError(result, err); },
                   [&result](const SuccessResult &pair) { testImportSuccess(result, pair); }},
               expectedResult);
}

void TestThumbnailsImporter::testImportError(const ImportResult &actual,
                                             const QFileDevice::FileError &expected) {
    QVERIFY(std::holds_alternative<QFileDevice::FileError>(actual));
    QCOMPARE(std::get<QFileDevice::FileError>(actual), expected);
}

void TestThumbnailsImporter::testImportSuccess(const ImportResult &actual,
                                               const SuccessResult &expected) {
    QVERIFY(std::holds_alternative<SuccessResult>(actual));

    const auto &result{std::get<SuccessResult>(actual)};
    QCOMPARE(result.second, expected.second);
    TestJsonMediaDeserializer::testMediaEqualityNoDate(result.first.getMedia(),
                                                       expected.first.getMedia());
}