#ifndef TESTMEDIUMUSERDATA_H
#define TESTMEDIUMUSERDATA_H

#include <QObject>

class TestMediumUserData : public QObject {
    Q_OBJECT

  private slots:
    static void testFavorite();

    static void testTopics_data();
    static void testTopics();

    static void testNotes_data();
    static void testNotes();

    static void testPriority_data();
    static void testPriority();
};

#endif
