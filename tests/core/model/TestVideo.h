#ifndef TESTVIDEO_H
#define TESTVIDEO_H

#include <QObject>

class TestVideo : public QObject {
    Q_OBJECT

  private slots:
    static void testCreate_data();
    static void testCreate();

    static void testVideoUrl_data();
    static void testVideoUrl();

    static void testDurationSeconds_data();
    static void testDurationSeconds();

    static void testUploadDate_data();
    static void testUploadDate();

    static void testThumbnailUrl_data();
    static void testThumbnailUrl();
};

#endif
