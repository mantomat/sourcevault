#include "TestThumbnailsExporter.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/Video.h"
#include "persistence/thumbnails/ThumbnailsExporter.h"
#include "shared/OverloadedVariantVisitor.h"
#include "shared/QUuidHasher.h"

#include <QTest>
#include <QUuid>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::Video;
using Core::Persistence::Thumbnails::ThumbnailsExporter;
using Core::Shared::OverloadedVariantVisitor;

using MediaGenerator = std::function<std::vector<std::unique_ptr<const Medium>>()>;

namespace {

const char *const thumbnailExtension{"jpg"};

}

void TestThumbnailsExporter::testExportLocalThumbnails_data() const {
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<QString>("destinationDirPath");
    QTest::addColumn<ExportResult>("expectedResult");

    QTest::addRow("Destination directory doesn't exist")
        << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }}
        << tempDir.filePath("non-existent-dir") << ExportResult{QFileDevice::PositionError};

    {
        QDir destination{tempDir.path()};
        QString destinationDirName{"empty-media"};
        assert(destination.mkdir(destinationDirName));
        assert(destination.cd(destinationDirName));

        QTest::addRow("Empty media, valid destination dir -> creates empty directory")
            << MediaGenerator{[] { return std::vector<std::unique_ptr<const Medium>>{}; }}
            << destination.path() << ExportResult{std::unordered_set<QUuid>{}};
    }

    {
        QDir destination{tempDir.path()};
        QString destinationDirName{"no-thumbnail-media"};
        assert(destination.mkdir(destinationDirName));
        assert(destination.cd(destinationDirName));

        QTest::addRow("Non-empty media but none has thumbnail, valid destination dir -> creates "
                      "empty directory")
            << MediaGenerator{[] {
                   std::vector<std::unique_ptr<const Medium>> media{};
                   media.push_back(Article::make("test").value());
                   media.push_back(Video::make("test").value());
                   media.push_back(Book::make("test").value());
                   return media;
               }}
            << destination.path() << ExportResult{std::unordered_set<QUuid>{}};
    }

    {
        QDir destination{tempDir.path()};
        QString destinationDirName{"with-thumbnail-media"};
        assert(destination.mkdir(destinationDirName));
        assert(destination.cd(destinationDirName));

        QDir thumbnailsSource{tempDir.path()};
        assert(thumbnailsSource.mkdir("valid-thumbnail-source"));
        assert(thumbnailsSource.cd("valid-thumbnail-source"));

        QString extension{thumbnailExtension};
        QString bookThumbnailName{"book-thumbnail." + extension};
        QString videoThumbnailName{"video-thumbnail." + extension};

        QFile bookThumbnail{thumbnailsSource.filePath(bookThumbnailName)};
        bookThumbnail.open(QFile::WriteOnly);
        QFile videoThumbnail{thumbnailsSource.filePath(videoThumbnailName)};
        videoThumbnail.open(QFile::WriteOnly);

        Book bookWithThumbnail{Book::create("test").value()};
        assert(bookWithThumbnail.thumbnailUrl().set(QUrl::fromLocalFile(bookThumbnail.fileName())));
        Video videoWithThumbnail{Video::create("test").value()};
        assert(
            videoWithThumbnail.thumbnailUrl().set(QUrl::fromLocalFile(videoThumbnail.fileName())));

        QTest::addRow("Multiple media have valid thumbnail, valid destination dir -> "
                      "correctly copies to destination")
            << MediaGenerator{[bookWithThumbnail, videoWithThumbnail] {
                   std::vector<std::unique_ptr<const Medium>> media{};
                   media.push_back(Article::make("test").value());
                   media.push_back(std::make_unique<Book>(bookWithThumbnail));
                   media.push_back(std::make_unique<Video>(videoWithThumbnail));
                   return media;
               }}
            << destination.path()
            << ExportResult{
                   std::unordered_set<QUuid>{bookWithThumbnail.id(), videoWithThumbnail.id()}};
    }

    {
        QDir destination{tempDir.path()};
        QString destinationDirName{"invalid-thumbnail-media"};
        assert(destination.mkdir(destinationDirName));
        assert(destination.cd(destinationDirName));

        QTest::addRow(
            "One media with invalid thumbnail, valid destination dir -> Nothing gets copied")
            << MediaGenerator{[this] {
                   auto withInvalidThumbnail(Book::make("test").value());
                   withInvalidThumbnail->thumbnailUrl().set(
                       QUrl::fromLocalFile(tempDir.filePath("non-existent.jpg")));

                   std::vector<std::unique_ptr<const Medium>> media{};
                   media.push_back(std::move(withInvalidThumbnail));
                   return media;
               }}
            << destination.path() << ExportResult{std::unordered_set<QUuid>{}};
    }
}

void TestThumbnailsExporter::testExportLocalThumbnails() {
    QFETCH(MediaGenerator, mediaGenerator);
    QFETCH(QString, destinationDirPath);
    QFETCH(ExportResult, expectedResult);

    auto media{mediaGenerator()};
    auto mediaPtrsView{media | std::views::transform([](const auto &m) { return m.get(); })};
    std::vector<const Medium *> mediaPtrs{mediaPtrsView.begin(), mediaPtrsView.end()};

    const ThumbnailsExporter exporter;

    const ExportResult result{exporter.exportLocalThumbnails(mediaPtrs, destinationDirPath)};

    std::visit(OverloadedVariantVisitor{
                   [&result](const QFileDevice::FileError &error) { testFileError(result, error); },
                   [&result, &destinationDirPath](const std::unordered_set<QUuid> &set) {
                       testSuccessfulExport(result, set, destinationDirPath);
                   },
               },
               expectedResult);
}

void TestThumbnailsExporter::testFileError(const ExportResult &actual,
                                           const QFileDevice::FileError &expected) {
    QVERIFY(std::holds_alternative<QFileDevice::FileError>(actual));
    QCOMPARE(std::get<QFileDevice::FileError>(actual), expected);
}
void TestThumbnailsExporter::testSuccessfulExport(const ExportResult &actual,
                                                  const std::unordered_set<QUuid> &expected,
                                                  const QString &destinationDirPath) {
    QVERIFY(std::holds_alternative<std::unordered_set<QUuid>>(actual));

    QDir destinationDir{destinationDirPath};
    QVERIFY(destinationDir.cd(ThumbnailsExporter::subDirName));

    for (const auto &id : expected) {
        QFileInfo info{destinationDir.filePath(id.toString(QUuid::StringFormat::WithoutBraces) +
                                               "." + QString{thumbnailExtension})};
        QVERIFY(info.exists());
        // we don't verify that it's the same as the original for now.
    }
}