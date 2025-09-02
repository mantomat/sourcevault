#ifndef TESTMEDIUMSERIALIZERVISITOR_H
#define TESTMEDIUMSERIALIZERVISITOR_H

#include <QObject>

class TestJsonMediumSerializerVisitor : public QObject {
    Q_OBJECT

  private slots:
    static void testVisitBook_data();
    static void testVisitBook();

    static void testVisitArticle_data();
    static void testVisitArticle();

    static void testVisitVideo_data();
    static void testVisitVideo();
};

#endif