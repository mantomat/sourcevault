#ifndef TESTVALIDATEDSET_H
#define TESTVALIDATEDSET_H

#include "model/ValidatedSet.h"

#include <QObject>
#include <QTest>
#include <algorithm>

using Core::Model::ValidatedSet;

class TestValidatedSet : public QObject {
    Q_OBJECT

  public:
    template <typename MediumClass, typename ElementsType>
    static void testValidatedFieldHelper(
        const std::function<bool(const ElementsType&)>& validator,
        const std::function<ValidatedSet<ElementsType>&(MediumClass&)>& setGetter,
        const std::function<const ValidatedSet<ElementsType>&(const MediumClass&)>& constSetGetter,
        const std::set<ElementsType>& candidateSet, bool shouldBeValid) {

        const bool didAllPassValidation{
            !candidateSet.empty() &&
            std::ranges::all_of(candidateSet, [&](const ElementsType& candidateElement) {
                return validator(candidateElement);
            })};
        QCOMPARE(didAllPassValidation, shouldBeValid);

        MediumClass mediumInstance{};
        ValidatedSet<ElementsType>& validatedSet{setGetter(mediumInstance)};

        const bool didSet{validatedSet.set(candidateSet)};
        QCOMPARE(didSet, shouldBeValid);

        const MediumClass& constMedium{mediumInstance};
        const auto& constValidatedSet{constSetGetter(constMedium)};

        const auto expectedValue{shouldBeValid ? std::make_optional(candidateSet) : std::nullopt};
        QCOMPARE(constValidatedSet.get(), expectedValue);
    }

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
