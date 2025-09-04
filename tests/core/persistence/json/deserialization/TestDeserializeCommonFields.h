#ifndef TESTDESERIALIZECOMMONFIELDS_H
#define TESTDESERIALIZECOMMONFIELDS_H

#include <QObject>

class TestDeserializeCommonFields : public QObject {
    Q_OBJECT

  private slots:
    static void testDeserializeCommonFields_data();
    static void testDeserializeCommonFields();
};

#endif