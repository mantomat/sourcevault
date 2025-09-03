#ifndef TESTJSONBOOKPARSER_H
#define TESTJSONBOOKPARSER_H

#include "model/Book.h"

#include <QObject>

using Core::Model::Book;

class TestJsonBookParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();

  public:
    static void testEqualityNoDate(const Book &lhs, const Book &rhs);
};

#endif