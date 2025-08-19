#ifndef TESTSORT_H
#define TESTSORT_H

#include "queries/sortings/Sort.h"

#include <QObject>

using Core::Queries::Sortings::Sort;

class ConcreteSort : public Sort {
  public:
    explicit ConcreteSort(bool isAscending = true)
        : Sort{isAscending} {}

    [[nodiscard]] auto apply(std::vector<const Medium *> media) const
        -> std::vector<const Medium *> override {
        return media;
    }

    [[nodiscard]] auto clone() const -> std::unique_ptr<Sort> override {
        return std::make_unique<ConcreteSort>(*this);
    }
};

class TestSort : public QObject {
    Q_OBJECT

  private slots:
    static void testConstructorAndIsAscending();

    static void testClone();
};

#endif