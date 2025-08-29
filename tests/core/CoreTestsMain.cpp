#include "model/TestArticle.h"
#include "model/TestBook.h"
#include "model/TestLibrary.h"
#include "model/TestMedium.h"
#include "model/TestMediumUserData.h"
#include "model/TestValidatedField.h"
#include "model/TestValidatedSet.h"
#include "model/TestVideo.h"
#include "queries/TestQueryBuilder.h"
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
    ret |= QTest::qExec(std::make_unique<TestQueryBuilder>().get(), argc, argv);
    // search
    ret |= QTest::qExec(std::make_unique<TestSearchScoreVisitor>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestFieldScoreCalculator>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestSearchEngine>().get(), argc, argv);
    // filters
    ret |= QTest::qExec(std::make_unique<TestFavoriteFilter>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMediumTypeFilter>().get(), argc, argv);
    // sortings
    ret |= QTest::qExec(std::make_unique<TestSort>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestDateAddedSort>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestTitleSort>().get(), argc, argv);

    return ret;
}