#include "TestVideo.h"

#include "model/Video.h"

#include <QTest>

using Core::Model::Video;

void TestVideo::testVideoUrlValidator_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty url should be invalid") << QUrl{} << false;
    QTest::addRow("Local url should be invalid") << QUrl::fromLocalFile("/etc/passwd") << false;
    QTest::addRow("Non-YouTube url should be invalid")
        << QUrl{"https://forum.qt.io/topic/56430/qurl-tolocalfile/3"} << false;
    QTest::addRow("YouTube non-video url should be invalid")
        << QUrl{"https://www.youtube.com/results?search_query=rickroll"} << false;
    QTest::addRow("YouTube video (watch) url should be valid")
        << QUrl{"https://www.youtube.com/watch?v=dQw4w9WgXcQ"} << true;
    QTest::addRow("YouTube video (share) url should be valid")
        << QUrl{"https://youtu.be/dQw4w9WgXcQ?si=BMhfEOItLGnp2kd4"} << true;
    QTest::addRow("Youtube video (share with start time) url should be valid")
        << QUrl{"https://youtu.be/dQw4w9WgXcQ?si=BMhfEOItLGnp2kd4&t=2"} << true;
}
void TestVideo::testVideoUrlValidator() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, expected);

    QCOMPARE(Video::videoUrlValidator(candidateUrl), expected);
}
void TestVideo::testVideoUrl() {
    Video video{};

    const QUrl validVideoUrl{"https://www.youtube.com/watch?v=dQw4w9WgXcQ"};
    video.videoUrl().set(validVideoUrl);
    const Video constVideoValidVideoUrl{video};
    QCOMPARE(constVideoValidVideoUrl.videoUrl().get(), std::make_optional(validVideoUrl));

    const QUrl invalidUrl{};
    video.videoUrl().set(invalidUrl);
    const Video constVideoInvalidVideoUrl{video};
    QCOMPARE(constVideoInvalidVideoUrl.videoUrl().get(), std::nullopt);
}

void TestVideo::testDurationSeconds() {
    Video video{};

    constexpr unsigned int validDurationSeconds{3456};
    video.durationSeconds().set(validDurationSeconds);
    const Video constVideoValidDurationSeconds{video};
    QCOMPARE(constVideoValidDurationSeconds.durationSeconds().get(),
             std::make_optional(validDurationSeconds));

    video.durationSeconds().unset();
    const Video constVideoInvalidDurationSeconds{video};
    QCOMPARE(constVideoInvalidDurationSeconds.durationSeconds().get(), std::nullopt);
}

void TestVideo::testUploadDateValidator_data() {
    QTest::addColumn<QDate>("candidateDate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Newly created dates should be invalid") << QDate{} << false;
    QTest::addRow("Any non-empty date should be valid") << QDate{2025, 04, 23} << true;
}
void TestVideo::testUploadDateValidator() {
    QFETCH(QDate, candidateDate);
    QFETCH(bool, expected);

    QCOMPARE(Video::uploadDateValidator(candidateDate), expected);
}
void TestVideo::testUploadDate() {
    Video video{};

    const QDate validUploadDate{2025, 04, 23};
    video.uploadDate().set(validUploadDate);
    const Video constVideoValidUploadDate{video};
    QCOMPARE(constVideoValidUploadDate.uploadDate().get(), std::make_optional(validUploadDate));

    const QDate invalidUploadDate{};
    video.uploadDate().set(invalidUploadDate);
    const Video constVideoInvalidUploadDate{video};
    QCOMPARE(constVideoInvalidUploadDate.uploadDate().get(), std::nullopt);
}

void TestVideo::testThumbnailUrlValidator_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty url should be invalid") << QUrl{} << false;
    QTest::addRow("Non-image url should be invalid")
        << QUrl{"https://www.youtube.com/watch?v=dQw4w9WgXcQ"} << false;
    QTest::addRow("YouTube thumbnail image url should be valid")
        << QUrl{"https://i.ytimg.com/vi/1m7PgAShgbk/sddefault.jpg"} << true;
    QTest::addRow("Any image url should be valid (for now)")
        << QUrl{"https://en.wikipedia.org/wiki/File:Image_created_with_a_mobile_phone.png"} << true;
    QTest::addRow("Local image url should be valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.png") << true;
}
void TestVideo::testThumbnailUrlValidator() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, expected);

    QCOMPARE(Video::thumbnailUrlValidator(candidateUrl), expected);
}
void TestVideo::testThumbnailUrl() {
    Video video{};

    const QUrl validThumbnailUrl{"https://i.ytimg.com/vi/1m7PgAShgbk/sddefault.jpg"};
    video.thumbnailUrl().set(validThumbnailUrl);
    const Video constVideoValidThumbnailUrl{video};
    QCOMPARE(constVideoValidThumbnailUrl.thumbnailUrl().get(),
             std::make_optional(validThumbnailUrl));

    const QUrl invalidUrl{};
    video.thumbnailUrl().set(invalidUrl);
    const Video constVideoInvalidThumbnailUrl{video};
    QCOMPARE(constVideoInvalidThumbnailUrl.thumbnailUrl().get(), std::nullopt);
}