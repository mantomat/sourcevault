#ifndef TESTSEARCHQUERY_H
#define TESTSEARCHQUERY_H

#include <QObject>

class TestSearchQuery : public QObject {
    Q_OBJECT
  private slots:

    static void testQuery_data();
    static void testQuery();
};

#endif