#ifndef TESTMEDIUMUSERDATA_H
#define TESTMEDIUMUSERDATA_H

#include <QObject>

class TestMediumUserData : public QObject {
    Q_OBJECT

  private slots:
    static void testFavorite();

    static void testTopicValidator_data();
    static void testTopicValidator();
    static void testTopics();

    static void testNotesValidator_data();
    static void testNotesValidator();
    static void testNotes();

    static void testPriority();
};

#endif
