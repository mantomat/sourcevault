#ifndef TESTJSONMEDIADESERIALIZER_H
#define TESTJSONMEDIADESERIALIZER_H

#include <QObject>

class TestJsonMediaDeserializer : public QObject {
    Q_OBJECT

  private slots:
    static void testDeserialize_data();
    static void testDeserialize();
};

#endif