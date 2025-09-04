#ifndef TESTJSONMEDIADESERIALIZER_H
#define TESTJSONMEDIADESERIALIZER_H

#include "model/Medium.h"

#include <QObject>

using Core::Model::Medium;

class TestJsonMediaDeserializer : public QObject {
    Q_OBJECT

  private slots:
    static void testDeserialize_data();
    static void testDeserialize();

  public:
    static void testMediaEqualityNoDate(std::vector<const Medium *> actual,
                                        std::vector<const Medium *> expected);
};

#endif