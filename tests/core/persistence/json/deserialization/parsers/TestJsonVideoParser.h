#ifndef TESTJSONVIDEOPARSER_H
#define TESTJSONVIDEOPARSER_H

#include <QObject>

class TestJsonVideoParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();
};

#endif