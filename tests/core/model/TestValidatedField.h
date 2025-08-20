#ifndef TESTVALIDATEDFIELD_H
#define TESTVALIDATEDFIELD_H

#include "model/ValidatedField.h"

#include <QObject>
#include <QTest>

using Core::Model::ValidatedField;

class TestValidatedField : public QObject {
    Q_OBJECT

  public:
    /**
     * A helper function to test media ValidatedField attributes.
     * It tests attribute-validator accordance, validator correctness and const-correctness.
     *
     * This helper, together with the tests that use it, don't test *units* in a strict sense (like
     * single methods). After all, it's testing three different methods. However, these three
     * methods are tightly coupled, each one is correct only when they are all correct. This is why
     * we're testing them together.
     */
    template <typename MediumClass, typename FieldType>
    static void testValidatedFieldHelper(
        const std::function<MediumClass()> &builder,
        const std::function<bool(const FieldType &)> &validator,
        const std::function<ValidatedField<FieldType> &(MediumClass &)> &fieldGetter,
        const std::function<const ValidatedField<FieldType> &(const MediumClass &)>
            &constFieldGetter,
        const FieldType &candidateValue, bool shouldBeValid) {

        const bool didPassValidation{validator(candidateValue)};
        QCOMPARE(didPassValidation, shouldBeValid);

        MediumClass mediumInstance = builder();
        ValidatedField<FieldType> &field{fieldGetter(mediumInstance)};

        const bool didSet{field.set(candidateValue)};
        QCOMPARE(didSet, shouldBeValid);

        const MediumClass &constMedium{mediumInstance};
        const auto &constField{constFieldGetter(constMedium)};

        const auto expectedValue{shouldBeValid ? std::make_optional(candidateValue) : std::nullopt};
        QCOMPARE(constField.get(), expectedValue);
    }

  private slots:

    static void testEqualsOperator_data();
    static void testEqualsOperator();

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
