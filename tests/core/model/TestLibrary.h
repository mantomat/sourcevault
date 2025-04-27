#ifndef TESTLIBRARY_H
#define TESTLIBRARY_H

#include <QObject>

class TestLibrary : public QObject {
    Q_OBJECT

  private slots:
    static void cleanup();

    static void testGetLibrary();
    static void testGetMediaView();
    static void testSetMedia();
    static void testSetMediaInvalid();
    static void testAddMedia();
    static void testAddMediaInvalid();
    static void testAddMedium();
    static void testAddMediumInvalid();
    static void testReplaceMedium();
    static void testReplaceMediumInvalid();
    static void testRemoveMedium();
    static void testClearMedia();
    static void testGetAllTopics();
};

#endif
