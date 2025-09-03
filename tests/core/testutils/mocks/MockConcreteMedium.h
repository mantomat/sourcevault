#ifndef MOCKCONCRETEMEDIUM_H
#define MOCKCONCRETEMEDIUM_H

#include "model/Medium.h"

using Core::Model::Medium;

class MockConcreteMedium final : public Medium {
  public:
    ~MockConcreteMedium() override = default;
    explicit MockConcreteMedium(QString title = "default title", QUuid id = QUuid::createUuid(),
                                QDate dateAdded = QDate::currentDate())
        : Medium{std::move(title), id, dateAdded} {};

    auto operator=(const MockConcreteMedium &) -> MockConcreteMedium & = default;
    auto operator=(MockConcreteMedium &&) -> MockConcreteMedium & = default;
    MockConcreteMedium(MockConcreteMedium &&m) = default;
    MockConcreteMedium(const MockConcreteMedium &m) = default;

    [[nodiscard]] auto operator==(const MockConcreteMedium &) const -> bool = default;

    [[nodiscard]] static auto make(QString title, QUuid id = QUuid::createUuid(),
                                   QDate dateAdded = QDate::currentDate()) {
        return std::make_optional(
            std::make_unique<MockConcreteMedium>(std::move(title), id, dateAdded));
    }

    [[nodiscard]] auto clone() const -> std::unique_ptr<Medium> override {
        return std::make_unique<MockConcreteMedium>(*this);
    }

    void accept(MediumVisitor & /*unused*/) const override {
        // never the case
    }

    friend class TestMedium;
};

#endif