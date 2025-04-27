#ifndef TESTVIDEO_H
#define TESTVIDEO_H

#include <QObject>

class TestVideo : public QObject {
    Q_OBJECT

  private slots:
    static void testVideoUrlValidator_data();
    static void testVideoUrlValidator();
    static void testVideoUrl();

    static void testDurationSeconds();

    static void testUploadDateValidator_data();
    static void testUploadDateValidator();
    static void testUploadDate();

    static void testThumbnailUrlValidator_data();
    static void testThumbnailUrlValidator();
    static void testThumbnailUrl();
};

#endif
