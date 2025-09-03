#ifndef TESTJSONARTICLEPARSER_H
#define TESTJSONARTICLEPARSER_H

#include "model/Article.h"

#include <QObject>

using Core::Model::Article;

class TestJsonArticleParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();

  public:
    static void testEqualityNoDate(const Article &lhs, const Article &rhs);
};

#endif