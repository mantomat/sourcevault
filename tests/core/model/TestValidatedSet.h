#ifndef TESTVALIDATEDSET_H
#define TESTVALIDATEDSET_H

#include <QObject>

class TestValidatedSet : public QObject {
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
    static void testSetWithMovedParameter_data();
    static void testSetWithMovedParameter();
    static void testOverwriteValue_data();
    static void testOverwriteValue();

    static void testAddValid_data();
    static void testAddValid();
    static void testAddInvalid_data();
    static void testAddInvalid();
    static void testAddParameterTypeCoercion_data();
    static void testAddParameterTypeCoercion();
    static void testAddWithMovedParameter_data();
    static void testAddWithMovedParameter();
    static void testAddAlreadyPresent_data();
    static void testAddAlreadyPresent();

    static void testRemovePresent_data();
    static void testRemovePresent();
    static void testRemoveAbsent_data();
    static void testRemoveAbsent();

    static void testGetValid_data();
    static void testGetValid();
    static void testGetUnset_data();
    static void testGetUnset();

    static void testSetUnsetTransition();
};

#endif
