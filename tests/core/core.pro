TEMPLATE = app
TARGET = testscore
CONFIG += c++20 console
CONFIG -= app_bundle

QT += testlib core

INCLUDEPATH += $$PWD ../../src/core

LIBS += -L"$$OUT_PWD/../../src/core" -lsourcevaultcore

HEADERS += \
    testutils/mocks/MockConcreteMedium.h \
    testutils/fixtures/JsonSerializationFixtures.h \
    shared/TestMediumTypeVisitor.h \
    model/TestMedium.h \
    model/TestValidatedField.h \
    model/TestValidatedSet.h \
    model/TestMediumUserData.h \
    model/TestVideo.h \
    model/TestBook.h \
    model/TestArticle.h \
    model/TestLibrary.h \
    queries/TestFilteringQuery.h \
    queries/TestSearchQuery.h \
    queries/TestSortingQuery.h \
    queries/search/TestFieldScoreCalculator.h \
    queries/search/TestSearchScoreVisitor.h \
    queries/search/TestSearchEngine.h \
    queries/filters/TestFavoriteFilter.h \
    queries/filters/TestMediumTypeFilter.h \
    queries/sortings/TestSort.h \
    queries/sortings/TestDateAddedSort.h \
    queries/sortings/TestTitleSort.h \
    persistence/json/TestJsonPersistenceManager.h \
    persistence/json/deserialization/TestJsonParsingLib.h \
    persistence/json/deserialization/TestJsonArticleParser.h \
    persistence/json/deserialization/TestJsonBookParser.h \
    persistence/json/deserialization/TestJsonVideoParser.h \
    persistence/json/deserialization/TestJsonMediaDeserializer.h \
    persistence/json/serialization/TestJsonMediumSerializerVisitor.h \
    persistence/json/serialization/TestJsonMediaSerializer.h \
    persistence/thumbnails/TestMediumThumbnailSetter.h \
    persistence/thumbnails/TestMediumThumbnailVisitor.h \
    persistence/thumbnails/TestThumbnailsExporter.h \
    persistence/thumbnails/TestThumbnailsImporter.h

SOURCES += \
    CoreTestsMain.cpp \
    shared/TestMediumTypeVisitor.cpp \
    model/TestMedium.cpp \
    model/TestValidatedField.cpp \
    model/TestValidatedSet.cpp \
    model/TestMediumUserData.cpp \
    model/TestVideo.cpp \
    model/TestBook.cpp \
    model/TestArticle.cpp \
    model/TestLibrary.cpp \
    queries/TestFilteringQuery.cpp \
    queries/TestSearchQuery.cpp \
    queries/TestSortingQuery.cpp \
    queries/search/TestFieldScoreCalculator.cpp \
    queries/search/TestSearchScoreVisitor.cpp \
    queries/search/TestSearchEngine.cpp \
    queries/filters/TestFavoriteFilter.cpp \
    queries/filters/TestMediumTypeFilter.cpp \
    queries/sortings/TestSort.cpp \
    queries/sortings/TestDateAddedSort.cpp \
    queries/sortings/TestTitleSort.cpp \
    persistence/json/TestJsonPersistenceManager.cpp \
    persistence/json/deserialization/TestJsonParsingLib.cpp \
    persistence/json/deserialization/TestJsonArticleParser.cpp \
    persistence/json/deserialization/TestJsonBookParser.cpp \
    persistence/json/deserialization/TestJsonVideoParser.cpp \
    persistence/json/deserialization/TestJsonMediaDeserializer.cpp \
    persistence/json/serialization/TestJsonMediumSerializerVisitor.cpp \
    persistence/json/serialization/TestJsonMediaSerializer.cpp \
    persistence/thumbnails/TestMediumThumbnailSetter.cpp \
    persistence/thumbnails/TestMediumThumbnailVisitor.cpp \
    persistence/thumbnails/TestThumbnailsExporter.cpp \
    persistence/thumbnails/TestThumbnailsImporter.cpp