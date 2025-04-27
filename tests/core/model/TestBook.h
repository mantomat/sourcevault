#ifndef TESTBOOK_H
#define TESTBOOK_H

#include <QObject>

class TestBook : public QObject {
    Q_OBJECT

  private slots:

    static void testIsbnValidator_data();
    static void testIsbnValidator();
    static void testIsbn();

    static void testEditionValidator_data();
    static void testEditionValidator();
    static void testEdition();

    static void testYearPublished();

    static void testPageNumber();

    static void testDescriptionValidator_data();
    static void testDescriptionValidator();
    static void testDescription();

    static void testThumbnailUrlValidator_data();
    static void testThumbnailUrlValidator();
    static void testThumbnailUrl();
};

#endif
