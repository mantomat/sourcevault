#ifndef TESTJSONMEDIASERIALIZER_H
#define TESTJSONMEDIASERIALIZER_H

#include <QObject>

class TestJsonMediaSerializer : public QObject {
    Q_OBJECT

  private slots:
    static void testSerialize_data();
    static void testSerialize();
};

#endif