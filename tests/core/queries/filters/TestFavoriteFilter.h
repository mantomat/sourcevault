#ifndef TESTFAVORITEFILTER_H
#define TESTFAVORITEFILTER_H

#include <QObject>

class TestFavoriteFilter : public QObject {
    Q_OBJECT

  private slots:
    static void testClone();

    static void testMatches_data();
    static void testMatches();
};

#endif