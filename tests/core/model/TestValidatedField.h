#ifndef TESTVALIDATEDFIELD_H
#define TESTVALIDATEDFIELD_H

#include <QObject>

class TestValidatedField : public QObject {
    Q_OBJECT

  private slots:

    static void testHas_data();
    static void testHas();

    static void testUnset_data();
    static void testUnset();

    static void testSetValid_data();
    static void testSetValid();
    static void testSetInvalid_data();
    static void testSetInvalid();
    static void testSetParameterTypeCoercion_data();
    static void testSetParameterTypeCoercion();
    static void testSetWithMovedParameter_data();
    static void testSetWithMovedParameter();
    static void testOverwriteValue_data();
    static void testOverwriteValue();

    static void testGetValid_data();
    static void testGetValid();
    static void testGetUnset_data();
    static void testGetUnset();

    static void testSetUnsetTransition();
};

#endif
