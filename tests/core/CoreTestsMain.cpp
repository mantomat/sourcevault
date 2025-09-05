#include "model/TestArticle.h"
#include "model/TestBook.h"
#include "model/TestLibrary.h"
#include "model/TestMedium.h"
#include "model/TestMediumUserData.h"
#include "model/TestValidatedField.h"
#include "model/TestValidatedSet.h"
#include "model/TestVideo.h"
#include "persistence/json/TestJsonPersistenceManager.h"
#include "persistence/json/deserialization/TestJsonArticleParser.h"
#include "persistence/json/deserialization/TestJsonBookParser.h"
#include "persistence/json/deserialization/TestJsonMediaDeserializer.h"
#include "persistence/json/deserialization/TestJsonParsingLib.h"
#include "persistence/json/deserialization/TestJsonVideoParser.h"
#include "persistence/json/serialization/TestJsonMediaSerializer.h"
#include "persistence/json/serialization/TestJsonMediumSerializerVisitor.h"
#include "persistence/thumbnails/TestMediumThumbnailSetter.h"
#include "persistence/thumbnails/TestMediumThumbnailVisitor.h"
#include "persistence/thumbnails/TestThumbnailsExporter.h"
#include "persistence/thumbnails/TestThumbnailsImporter.h"
#include "queries/TestFilteringQuery.h"
#include "queries/TestSearchQuery.h"
#include "queries/TestSortingQuery.h"
#include "queries/filters/TestFavoriteFilter.h"
#include "queries/filters/TestMediumTypeFilter.h"
#include "queries/search/TestFieldScoreCalculator.h"
#include "queries/search/TestSearchEngine.h"
#include "queries/search/TestSearchScoreVisitor.h"
#include "queries/sortings/TestDateAddedSort.h"
#include "queries/sortings/TestSort.h"
#include "queries/sortings/TestTitleSort.h"
#include "shared/TestMediumTypeVisitor.h"

#include <QTest>
#include <qtestcase.h>

auto main(const int argc, char **argv) -> int {
    int ret = 0;

    // model
    ret |= QTest::qExec(std::make_unique<TestValidatedField>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestValidatedSet>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMediumUserData>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMedium>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestVideo>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestBook>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestArticle>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestLibrary>().get(), argc, argv);

    // shared
    ret |= QTest::qExec(std::make_unique<TestMediumTypeVisitor>().get(), argc, argv);

    // queries
    ret |= QTest::qExec(std::make_unique<TestFilteringQuery>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestSearchQuery>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestSortingQuery>().get(), argc, argv);
    //// search
    ret |= QTest::qExec(std::make_unique<TestSearchScoreVisitor>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestFieldScoreCalculator>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestSearchEngine>().get(), argc, argv);
    //// filters
    ret |= QTest::qExec(std::make_unique<TestFavoriteFilter>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMediumTypeFilter>().get(), argc, argv);
    //// sortings
    ret |= QTest::qExec(std::make_unique<TestSort>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestDateAddedSort>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestTitleSort>().get(), argc, argv);

    // persistence
    //// json
    ret |= QTest::qExec(std::make_unique<TestJsonPersistenceManager>().get(), argc, argv);
    ///// serialization
    ret |= QTest::qExec(std::make_unique<TestJsonMediumSerializerVisitor>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestJsonMediaSerializer>().get(), argc, argv);
    ////// deserialization
    ret |= QTest::qExec(std::make_unique<TestJsonMediaDeserializer>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestJsonParsingLib>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestJsonArticleParser>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestJsonBookParser>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestJsonVideoParser>().get(), argc, argv);
    //// thumbnails
    ret |= QTest::qExec(std::make_unique<TestMediumThumbnailSetter>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMediumThumbnailVisitor>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestThumbnailsExporter>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestThumbnailsImporter>().get(), argc, argv);

    return ret;
}