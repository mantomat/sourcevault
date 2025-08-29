#ifndef TESTSEARCHENGINE_H
#define TESTSEARCHENGINE_H

#include <QObject>

class TestSearchEngine : public QObject {
    Q_OBJECT
  private slots:
    static void testSearch_data();
    static void testSearch();
};

#endif