#ifndef TESTMEDIUMTYPEVISITOR_H
#define TESTMEDIUMTYPEVISITOR_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"
#include "shared/MediumTypeVisitor.h"

#include <QObject>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;
using Core::Shared::MediumTypeVisitor;

class TestMediumTypeVisitor : public QObject {
    Q_OBJECT

    MediumTypeVisitor bookVisitor{{typeid(Book)}};
    MediumTypeVisitor articleVisitor{{typeid(Article)}};
    MediumTypeVisitor videoVisitor{{typeid(Video)}};

  private slots:
    static void testHasMatched_data();
    static void testHasMatched();

    void testVisitBook();

    void testVisitArticle();

    void testVisitVideo();
};

#endif