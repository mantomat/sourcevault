#ifndef TESTOPTIONALFIELD_H
#define TESTOPTIONALFIELD_H

#include <QObject>

class TestOptionalField : public QObject {
    Q_OBJECT

  private slots:

    static void testHas_data();
    static void testHas();

    static void testGet_data();
    static void testGet();

    static void testSet_data();
    static void testSet();

    static void testUnset_data();
    static void testUnset();
};

#endif
