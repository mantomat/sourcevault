#include "TestJsonPersistenceManager.h"

#include "persistence/json/JsonPersistenceManager.h"
#include "persistence/json/deserialization/JsonDeserializationError.h"
#include "persistence/json/deserialization/TestJsonMediaDeserializer.h"
#include "testutils/fixtures/JsonSerializationFixtures.h"

#include <QJsonDocument>
#include <QTemporaryFile>
#include <QTest>
#include <qdebug.h>

using Core::Persistence::Json::JsonDeserializationError;
using Core::Persistence::Json::JsonPersistenceManager;

void TestJsonPersistenceManager::testImportLibraryFileError() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString nonExistentFilePath{tempDir.path() + "/a_file_that_does_not_exist.json"};

    JsonPersistenceManager manager;
    const auto result{manager.importLibrary(nonExistentFilePath)};

    QVERIFY(std::holds_alternative<QFileDevice::FileError>(result));
}

void TestJsonPersistenceManager::testImportLibraryMalformedJson() {
    QTemporaryFile file;
    file.open();

    const char *syntacticallyInvalidJson{R"({"version" = "1.0'})"};
    file.write(syntacticallyInvalidJson);
    file.close();

    JsonPersistenceManager manager;
    const auto result{manager.importLibrary(file.fileName())};

    QVERIFY(std::holds_alternative<JsonDeserializationError>(result));
    QCOMPARE(std::get<JsonDeserializationError>(result).code,
             JsonDeserializationError::Code::InvalidJson);
}

void TestJsonPersistenceManager::testImportLibraryLogicallyInvalidJson() {
    QTemporaryFile file;
    file.open();

    const char *logicallyInvalidJson{R"({})"};
    file.write(logicallyInvalidJson);
    file.close();

    JsonPersistenceManager manager;
    const auto result{manager.importLibrary(file.fileName())};

    QVERIFY(std::holds_alternative<JsonDeserializationError>(result));
}

void TestJsonPersistenceManager::testImportLibrarySuccess() {
    QTemporaryFile file;
    file.open();

    JsonLibraryFixture fixture;
    file.write(fixture.json.toJson());
    file.close();

    JsonPersistenceManager manager;
    const auto result{manager.importLibrary(file.fileName())};

    QVERIFY(std::holds_alternative<Library>(result));
    auto actual{std::get<Library>(result).getMedia()};
    auto expected{fixture.library.getMedia()};

    TestJsonMediaDeserializer::testMediaEqualityNoDate(std::get<Library>(result).getMedia(),
                                                       fixture.library.getMedia());
}

void TestJsonPersistenceManager::testExportMediaSuccess() {
    QTemporaryFile file;

    JsonPersistenceManager manager;
    JsonLibraryFixture fixture;

    file.open();
    file.write(fixture.json.toJson());
    file.close();

    auto result{manager.exportMedia(fixture.library.getMedia(), file.fileName())};

    QVERIFY(!result.has_value());
}

void TestJsonPersistenceManager::testRoundTrip() {
    const JsonLibraryFixture fixture;
    const JsonPersistenceManager manager;

    QTemporaryDir tempDir;
    QVERIFY2(tempDir.isValid(), "Temporary directory creation failed.");

    QString firstFilePath{tempDir.path() + "exp1.json"};
    const auto firstExportResult{manager.exportMedia(fixture.library.getMedia(), firstFilePath)};
    QVERIFY(!firstExportResult.has_value());

    const auto firstImportResult{manager.importLibrary(firstFilePath)};
    QVERIFY(std::holds_alternative<Library>(firstImportResult));
    const auto firstImportedLibrary{std::get<Library>(firstImportResult)};

    QString secondFilePath{tempDir.path() + "exp2.json"};
    const auto secondExportResult{
        manager.exportMedia(firstImportedLibrary.getMedia(), secondFilePath)};
    QVERIFY(!secondExportResult.has_value());

    const auto secondImportResult(manager.importLibrary(secondFilePath));
    QVERIFY(std::holds_alternative<Library>(firstImportResult));
    const auto secondImportedLibrary{std::get<Library>(firstImportResult)};

    QFile firstExport{firstFilePath};
    firstExport.open(QIODevice::ReadOnly);
    QFile secondExport{secondFilePath};
    secondExport.open(QIODevice::ReadOnly);

    QCOMPARE(firstExport.readAll(), secondExport.readAll());
    TestJsonMediaDeserializer::testMediaEqualityNoDate(secondImportedLibrary.getMedia(),
                                                       firstImportedLibrary.getMedia());
}