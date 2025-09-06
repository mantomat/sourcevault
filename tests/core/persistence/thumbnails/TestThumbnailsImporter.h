// TODO
#ifndef TESTTHUMBNAILSIMPORTER_H
#define TESTTHUMBNAILSIMPORTER_H

#include "model/Library.h"

#include <QFileDevice>
#include <QObject>
#include <QTemporaryDir>
#include <unordered_set>

using Core::Model::Library;

class TestThumbnailsImporter : public QObject {
    Q_OBJECT

    QTemporaryDir tempDir;

  private slots:
    void testImportLocalThumbnails_data() const;
    static void testImportLocalThumbnails();

  public:
    using SuccessResult = std::pair<Library, std::unordered_set<QUuid>>;
    using ImportResult = std::variant<QFileDevice::FileError, SuccessResult>;

  private:
    static void testImportError(const ImportResult &actual, const QFileDevice::FileError &expected);
    static void testImportSuccess(const ImportResult &actual, const SuccessResult &expected);
};

#endif