#ifndef TESTFILTERINGQUERY_H
#define TESTFILTERINGQUERY_H

#include <QObject>

class TestFilteringQuery : public QObject {
    Q_OBJECT

  private slots:
    static void testCopyConstructor();

    static void testCopyAssignment();

    static void testCreate_data();
    static void testCreate();

    static void testQuery_data();
    static void testQuery();
};

#endif