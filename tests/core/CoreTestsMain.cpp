#include "model/TestArticle.h"
#include "model/TestBook.h"
#include "model/TestLibrary.h"
#include "model/TestMedium.h"
#include "model/TestMediumUserData.h"
#include "model/TestValidatedField.h"
#include "model/TestValidatedSet.h"
#include "model/TestVideo.h"
#include "queries/TestQueryBuilder.h"

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

    // queries
    ret |= QTest::qExec(std::make_unique<TestQueryBuilder>().get(), argc, argv);

    return ret;
}