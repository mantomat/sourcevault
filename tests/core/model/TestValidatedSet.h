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
        const std::function<MediumClass()>& builder,
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

        MediumClass mediumInstance = builder();
        ValidatedSet<ElementsType>& validatedSet{setGetter(mediumInstance)};

        const bool didSet{validatedSet.set(candidateSet)};
        QCOMPARE(didSet, shouldBeValid);

        const MediumClass& constMedium{mediumInstance};
        const auto& constValidatedSet{constSetGetter(constMedium)};

        const auto expectedValue{shouldBeValid ? std::make_optional(candidateSet) : std::nullopt};
        QCOMPARE(constValidatedSet.get(), expectedValue);
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

    static void testAdd_data();
    static void testAdd();

    static void testRemove_data();
    static void testRemove();

    static void testUnset_data();
    static void testUnset();
};

#endif
