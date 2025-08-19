#ifndef TESTDATEADDEDSORT_H
#define TESTDATEADDEDSORT_H

#include <QObject>

class TestDateAddedSort : public QObject {
    Q_OBJECT

  private slots:

    static void testConstructorAndIsAscending();

    static void testClone();

    static void testApply_data();
    static void testApply();
};

#endif