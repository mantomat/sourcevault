#ifndef TESTJSONPERSISTENCEMANAGER_H
#define TESTJSONPERSISTENCEMANAGER_H

#include "model/Library.h"
#include "persistence/json/deserialization/JsonDeserializationError.h"

#include <QFileDevice>
#include <QObject>
#include <QTemporaryFile>

using Core::Model::Library;
using Core::Persistence::Json::JsonDeserializationError;
using ImportResult = std::variant<QFileDevice::FileError, JsonDeserializationError, Library>;

class TestJsonPersistenceManager : public QObject {
    Q_OBJECT

  private slots:
    static void testImportLibraryFileError();
    static void testImportLibraryMalformedJson();
    static void testImportLibraryLogicallyInvalidJson();
    static void testImportLibrarySuccess();

    // we are not testing the export error since I'm unable to recreate the required environment
    // safely. Skill issue :/ ~mantomat
    static void testExportMediaSuccess();

    static void testRoundTrip();
};

#endif