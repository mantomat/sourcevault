#ifndef TESTMEDIUMTYPEFILTER_H
#define TESTMEDIUMTYPEFILTER_H

#include <QObject>

class TestMediumTypeFilter : public QObject {
    Q_OBJECT

  private slots:
    static void testClone();

    static void testMatches_data();
    static void testMatches();
};

#endif