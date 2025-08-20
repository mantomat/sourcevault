#include "TestVideo.h"

#include "TestMedium.h"
#include "TestValidatedField.h"
#include "model/Video.h"

#include <QTest>

using Core::Model::Video;

const auto videoBuilder{[] { return Video::create("default title").value(); }};

void TestVideo::testClone() {
    auto videoToClone{std::make_unique<Video>(videoBuilder())};
    videoToClone->authors().add("me");
    videoToClone->durationSeconds().set(1);
    videoToClone->userData().favorite() = false;

    auto clone{videoToClone->clone()};

    QVERIFY(dynamic_cast<Video *>(clone.get()) != nullptr);
    QVERIFY(videoToClone != clone);
    QCOMPARE(*videoToClone, *clone);
}

void TestVideo::testCreate_data() {
    TestMedium::testCreateData();
}

void TestVideo::testCreate() {
    QFETCH(bool, shouldBeValid);
    QFETCH(QString, candidateTitle);
    QFETCH(QUuid, candidateId);
    QFETCH(QDate, candidateDateAdded);

    const auto optionalVideo{Video::create(candidateTitle, candidateId, candidateDateAdded)};

    if (!shouldBeValid) {
        QVERIFY2(!optionalVideo.has_value(), "The video mustn't be created");
        return;
    }

    QVERIFY2(optionalVideo.has_value(), "The video must be created");
    QCOMPARE(optionalVideo.value().title(), candidateTitle);
    QCOMPARE(optionalVideo.value().id(), candidateId);
    QCOMPARE(optionalVideo.value().dateAdded(), candidateDateAdded);
}

void TestVideo::testVideoUrl_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty urls are invalid") << QUrl{} << false;
    QTest::addRow("Local urls are invalid") << QUrl::fromLocalFile("/etc/passwd") << false;
    QTest::addRow("Non-YouTube urls are invalid")
        << QUrl{"https://forum.qt.io/topic/56430/qurl-tolocalfile/3"} << false;
    QTest::addRow("YouTube non-video urls are invalid")
        << QUrl{"https://www.youtube.com/results?search_query=rickroll"} << false;
    QTest::addRow("Only https urls are valid - i.e. http is invalid")
        << QUrl{"http://www.youtube.com/watch?v=dQw4w9WgXcQ"} << false;

    QTest::addRow("YouTube video (watch) urls are valid")
        << QUrl{"https://www.youtube.com/watch?v=dQw4w9WgXcQ"} << true;
    QTest::addRow("YouTube video (share) urls are valid")
        << QUrl{"https://youtu.be/dQw4w9WgXcQ?si=BMhfEOItLGnp2kd4"} << true;
    QTest::addRow("Youtube video (share with start time) urls are valid")
        << QUrl{"https://youtu.be/dQw4w9WgXcQ?si=BMhfEOItLGnp2kd4&t=2"} << true;
}

void TestVideo::testVideoUrl() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Video, QUrl>(
        videoBuilder, &Video::videoUrlValidator,
        [](Video &v) -> ValidatedField<QUrl> & { return v.videoUrl(); },
        [](const Video &v) -> const ValidatedField<QUrl> & { return v.videoUrl(); }, candidateUrl,
        shouldBeValid);
}

void TestVideo::testDurationSeconds_data() {
    QTest::addColumn<unsigned int>("candidateDurationSeconds");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Zero seconds is an invalid video duration") << 0u << false;
    QTest::addRow("Every integer greater than zero is a valid video duration") << 1u << true;
    QTest::addRow("Every integer greater than zero is a valid video duration") << 100u << true;
}

void TestVideo::testDurationSeconds() {
    QFETCH(unsigned int, candidateDurationSeconds);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Video, unsigned int>(
        videoBuilder, &Video::durationSecondsValidator,
        [](Video &v) -> ValidatedField<unsigned int> & { return v.durationSeconds(); },
        [](const Video &v) -> const ValidatedField<unsigned int> & { return v.durationSeconds(); },
        candidateDurationSeconds, shouldBeValid);
}

void TestVideo::testUploadDate_data() {
    QTest::addColumn<QDate>("candidateUploadDate");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty dates are invalid") << QDate{} << false;
    QTest::addRow("Dates before 2005-01-01 are invalid") << QDate{2004, 12, 31} << false;
    QTest::addRow("Any non-empty date after 2005-01-01 (included) is valid")
        << QDate{2005, 1, 1} << true;
    QTest::addRow("Any non-empty date after 2005-01-01 is valid") << QDate{2025, 04, 23} << true;
}

void TestVideo::testUploadDate() {
    QFETCH(QDate, candidateUploadDate);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Video, QDate>(
        videoBuilder, &Video::uploadDateValidator,
        [](Video &v) -> ValidatedField<QDate> & { return v.uploadDate(); },
        [](const Video &v) -> const ValidatedField<QDate> & { return v.uploadDate(); },
        candidateUploadDate, shouldBeValid);
}

void TestVideo::testThumbnailUrl_data() {
    QTest::addColumn<QUrl>("candidateThumbnailUrl");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty urls are invalid") << QUrl{} << false;

    QTest::addRow("Only local paths, http and https - i.e. mailto is not")
        << QUrl{"mailto:user@example.com"} << false;
    QTest::addRow("Only local paths, http and https - i.e. ftp is not")
        << QUrl{"ftp://user@host/foo/image.png"} << false;

    QTest::addRow("Non-image http urls are invalid")
        << QUrl{"http://www.example.com/book"} << false;
    QTest::addRow("Non-image https urls are invalid")
        << QUrl{"https://www.example.com/book"} << false;
    QTest::addRow("Non-image file urls are invalid")
        << QUrl{"file://www.example.com/book"} << false;
    QTest::addRow("Non-image local paths are invalid")
        << QUrl::fromLocalFile("/etc/passwd") << false;

    QTest::addRow(".jpg image http urls are valid") << QUrl{"http://example.com/image.jpg"} << true;
    QTest::addRow(".jpeg image http urls are valid")
        << QUrl{"http://example.com/image.jpeg"} << true;
    QTest::addRow(".png image http urls are valid")
        << QUrl{"http://upload.wikimedia.org/wikipedia/commons/4/47/"
                "PNG_transparency_demonstration_1.png"}
        << true;

    QTest::addRow(".jpg image https urls are valid")
        << QUrl{"https://example.com/image.jpg"} << true;
    QTest::addRow(".jpeg image https urls are valid")
        << QUrl{"https://example.com/image.jpeg"} << true;
    QTest::addRow(".png image https urls are valid")
        << QUrl{"https://upload.wikimedia.org/wikipedia/commons/4/47/"
                "PNG_transparency_demonstration_1.png"}
        << true;
    QTest::addRow("YouTube thumbnail image url should be valid")
        << QUrl{"https://i.ytimg.com/vi/1m7PgAShgbk/sddefault.jpg"} << true;

    QTest::addRow(".jpg local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.jpg") << true;
    QTest::addRow(".jpeg local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.jpeg") << true;
    QTest::addRow(".png local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.png") << true;
}

void TestVideo::testThumbnailUrl() {
    QFETCH(QUrl, candidateThumbnailUrl);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Video, QUrl>(
        videoBuilder, &Video::thumbnailUrlValidator,
        [](Video &v) -> ValidatedField<QUrl> & { return v.thumbnailUrl(); },
        [](const Video &v) -> const ValidatedField<QUrl> & { return v.thumbnailUrl(); },
        candidateThumbnailUrl, shouldBeValid);
}