#ifndef TESTSORTINGQUERY_H
#define TESTSORTINGQUERY_H

#include <QObject>

class TestSortingQuery : public QObject {
    Q_OBJECT
  private slots:

    static void testCopyConstruction();

    static void testCopyAssignment();

    static void testCreate_data();
    static void testCreate();

    static void testQuery_data();
    static void testQuery();
};

#endif