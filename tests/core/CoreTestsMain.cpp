#include "model/TestArticle.h"
#include "model/TestBook.h"
#include "model/TestLibrary.h"
#include "model/TestMedium.h"
#include "model/TestMediumUserData.h"
#include "model/TestOptionalField.h"
#include "model/TestValidatedField.h"
#include "model/TestValidatedSet.h"
#include "model/TestVideo.h"

#include <QTest>

int main(const int argc, char** argv) {
    int ret = 0;

    ret |= QTest::qExec(std::make_unique<TestValidatedField>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestValidatedSet>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestOptionalField>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMediumUserData>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestMedium>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestVideo>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestBook>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestArticle>().get(), argc, argv);
    ret |= QTest::qExec(std::make_unique<TestLibrary>().get(), argc, argv);

    return ret;
}