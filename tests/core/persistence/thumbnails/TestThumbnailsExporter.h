#ifndef TESTTHUMBNAILSEXPORTER_H
#define TESTTHUMBNAILSEXPORTER_H

#include <QObject>
#include <QTemporaryDir>
#include <unordered_set>

class TestThumbnailsExporter : public QObject {
    Q_OBJECT

    QTemporaryDir tempDir;

  private slots:
    void testExportLocalThumbnails_data() const;
    static void testExportLocalThumbnails();

  public:
    using ExportResult = std::variant<QFileDevice::FileError, std::unordered_set<QUuid>>;

  private:
    static void testFileError(const ExportResult &actual, const QFileDevice::FileError &expected);
    static void testSuccessfulExport(const ExportResult &actual,
                                     const std::unordered_set<QUuid> &expected,
                                     const QString &destinationDirPath);
};

#endif