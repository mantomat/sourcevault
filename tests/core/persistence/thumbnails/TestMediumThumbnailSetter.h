#ifndef TESTMEDIUMTHUMBNAILSETTER_H
#define TESTMEDIUMTHUMBNAILSETTER_H

#include <QObject>
#include <QUrl>

class TestMediumThumbnailSetter : public QObject {
    Q_OBJECT

    QUrl validThumbnailUrl{"https://i.ytimg.com/vi/CxVXvFOPIyQ/hqdefault.jpg"};
    QUrl invalidThumbnailUrl{"https://i.ytimg.com/vi/CxVXvFOPIyQ/hqdefault.gif"};

  private slots:
    void initTestCase() const;

    void testVisitArticle_data() const;
    static void testVisitArticle();

    void testVisitBook_data() const;
    static void testVisitBook();

    void testVisitVideo_data() const;
    static void testVisitVideo();
};

#endif