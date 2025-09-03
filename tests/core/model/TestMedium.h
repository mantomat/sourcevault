#ifndef TESTMEDIUM_H
#define TESTMEDIUM_H

#include <QObject>

class TestMedium final : public QObject {
    Q_OBJECT
  public:
    static void testCreateData();

  private slots:
    static void testCreateValidator_data();
    static void testCreateValidator();

    static void testId();

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
