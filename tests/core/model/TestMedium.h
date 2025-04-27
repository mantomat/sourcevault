#ifndef TESTMEDIUM_H
#define TESTMEDIUM_H

#include "model/Book.h"
#include "model/Medium.h"

#include <QObject>

class ConcreteMedium final : public Core::Model::Medium {
  public:
    ~ConcreteMedium() override = default;
};

class TestMedium final : public QObject {
    Q_OBJECT

  private slots:
    static void testUserData();

    static void testDateAddedValidator_data();
    static void testDateAddedValidator();
    static void testDateAdded();

    static void testTitleValidator_data();
    static void testTitleValidator();
    static void testTitle();

    static void testAuthorValidator_data();
    static void testAuthorValidator();
    static void testAuthors();

    static void testLanguageValidator_data();
    static void testLanguageValidator();
    static void testLanguage();
};

#endif
