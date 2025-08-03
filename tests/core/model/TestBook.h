#ifndef TESTBOOK_H
#define TESTBOOK_H

#include <QObject>

class TestBook : public QObject {
    Q_OBJECT

  private slots:

    static void testIsbn_data();
    static void testIsbn();

    static void testEdition_data();
    static void testEdition();

    static void testPublisher_data();
    static void testPublisher();

    static void testYearPublished_data();
    static void testYearPublished();

    static void testPageNumber_data();
    static void testPageNumber();

    static void testDescription_data();
    static void testDescription();

    static void testThumbnailUrl_data();
    static void testThumbnailUrl();
};

#endif
