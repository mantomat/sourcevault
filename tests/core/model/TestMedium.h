#ifndef TESTMEDIUM_H
#define TESTMEDIUM_H

#include "model/Book.h"
#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

class ConcreteMedium final : public Medium {
  public:
    ~ConcreteMedium() override = default;
    explicit ConcreteMedium(QString title = "default title", QUuid id = QUuid::createUuid(),
                            QDate dateAdded = QDate::currentDate())
        : Medium{std::move(title), std::move(id), std::move(dateAdded)} {};

    ConcreteMedium(ConcreteMedium&& m) = default;
    ConcreteMedium(const ConcreteMedium& m) = default;

    std::unique_ptr<Medium> clone() const override {
        return std::make_unique<ConcreteMedium>(*this);
    }

    friend class TestMedium;
};

class TestMedium final : public QObject {
    Q_OBJECT
  public:
    static void testCreateData();

  private slots:
    static void testCreateValidator_data();
    static void testCreateValidator();

    static void testId();

    static void testUserData();

    static void testDateAdded_data();
    static void testDateAdded();

    static void testTitle_data();
    static void testTitle();

    static void testAuthors_data();
    static void testAuthors();

    static void testLanguage_data();
    static void testLanguage();
};

#endif
