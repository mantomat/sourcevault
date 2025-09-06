#ifndef TESTMEDIUMTHUMBNAILVISITOR_H
#define TESTMEDIUMTHUMBNAILVISITOR_H

#include <QObject>
#include <QTemporaryDir>

class TestMediumThumbnailVisitor : public QObject {
    Q_OBJECT

    QTemporaryDir tempDir;

  private slots:
    static void testVisitArticle_data();
    static void testVisitArticle();

    void testVisitBook_data() const;
    static void testVisitBook();

    void testVisitVideo_data() const;
    static void testVisitVideo();
};

#endif