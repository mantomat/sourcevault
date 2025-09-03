#ifndef TESTJSONARTICLEPARSER_H
#define TESTJSONARTICLEPARSER_H

#include <QObject>

class TestJsonArticleParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();
};

#endif