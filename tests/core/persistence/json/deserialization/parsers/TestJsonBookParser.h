#ifndef TESTJSONBOOKPARSER_H
#define TESTJSONBOOKPARSER_H

#include <QObject>

class TestJsonBookParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();
};

#endif