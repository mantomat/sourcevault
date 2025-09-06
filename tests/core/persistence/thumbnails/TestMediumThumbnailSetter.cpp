#include "TestMediumThumbnailSetter.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "persistence/thumbnails/MediumThumbnailSetter.h"

#include <QTest>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Persistence::Thumbnails::MediumThumbnailSetter;

void TestMediumThumbnailSetter::initTestCase() const {
    assert(Book::thumbnailUrlValidator(validThumbnailUrl));
    assert(Video::thumbnailUrlValidator(validThumbnailUrl));
    assert(!Book::thumbnailUrlValidator(invalidThumbnailUrl));
    assert(!Video::thumbnailUrlValidator(invalidThumbnailUrl));
}

void TestMediumThumbnailSetter::testVisitArticle_data() const {
    QTest::addColumn<Article>("article");
    QTest::addColumn<QUrl>("url");

    QTest::addRow("Empty article, valid url - should never set the url")
        << Article::create("test").value() << validThumbnailUrl;

    QTest::addRow("Empty article, invalid url - should never set the url")
        << Article::create("test").value() << invalidThumbnailUrl;
}
void TestMediumThumbnailSetter::testVisitArticle() {
    QFETCH(Article, article);
    QFETCH(QUrl, url);

    MediumThumbnailSetter setter;
    setter.setThumbnailUrl(url);

    article.accept(setter);
    QCOMPARE(setter.wasSuccessful(), false);
}

void TestMediumThumbnailSetter::testVisitBook_data() const {
    QTest::addColumn<Book>("book");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("shouldSet");

    QTest::addRow("Empty book, valid url -> url should be set")
        << Book::create("test").value() << validThumbnailUrl << true;

    QTest::addRow("Empty book, invalid url -> url should not be set")
        << Book::create("test").value() << invalidThumbnailUrl << false;

    Book withThumbnail{Book::create("test").value()};
    withThumbnail.thumbnailUrl().set(validThumbnailUrl);

    QTest::addRow("Book already has thumbnail, valid url -> url be overridden")
        << withThumbnail << validThumbnailUrl << true;

    QTest::addRow("Book already has thumbnail, invalid url -> url should not be set")
        << withThumbnail << invalidThumbnailUrl << false;
}
void TestMediumThumbnailSetter::testVisitBook() {
    QFETCH(Book, book);
    QFETCH(QUrl, url);
    QFETCH(bool, shouldSet);

    MediumThumbnailSetter setter;
    setter.setThumbnailUrl(url);

    book.accept(setter);
    QCOMPARE(setter.wasSuccessful(), shouldSet);
    if (shouldSet) {
        QCOMPARE(book.thumbnailUrl().get(), url);
    }
}

void TestMediumThumbnailSetter::testVisitVideo_data() const {
    QTest::addColumn<Video>("video");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("shouldSet");

    QTest::addRow("Empty video, valid url -> url should be set")
        << Video::create("test").value() << validThumbnailUrl << true;

    QTest::addRow("Empty video, invalid url -> url should not be set")
        << Video::create("test").value() << invalidThumbnailUrl << false;

    Video withThumbnail{Video::create("test").value()};
    withThumbnail.thumbnailUrl().set(validThumbnailUrl);

    QTest::addRow("Video already has thumbnail, valid url -> url be overridden")
        << withThumbnail << validThumbnailUrl << true;

    QTest::addRow("Video already has thumbnail, invalid url -> url should not be set")
        << withThumbnail << invalidThumbnailUrl << false;
}
void TestMediumThumbnailSetter::testVisitVideo() {
    QFETCH(Video, video);
    QFETCH(QUrl, url);
    QFETCH(bool, shouldSet);

    MediumThumbnailSetter setter;
    setter.setThumbnailUrl(url);

    video.accept(setter);
    QCOMPARE(setter.wasSuccessful(), shouldSet);
    if (shouldSet) {
        QCOMPARE(video.thumbnailUrl().get(), url);
    }
}