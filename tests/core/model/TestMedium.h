#ifndef TESTMEDIUM_H
#define TESTMEDIUM_H

#include "model/Book.h"
#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

class ConcreteMedium final : public Medium {
  public:
    ~ConcreteMedium() override = default;
};

class TestMedium final : public QObject {
    Q_OBJECT

  private slots:
    static void testUserData();

    static void testDateAdded_data();
    static void testDateAdded();

    static void testTitle_data();
    static void testTitle();

    static void testAuthors_data();
    static void testAuthors();

    static void testLanguage_data();
    static void testLanguage();
};

#endif
