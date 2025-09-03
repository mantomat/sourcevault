#ifndef TESTJSONVIDEOPARSER_H
#define TESTJSONVIDEOPARSER_H

#include "model/Video.h"

#include <QObject>

using Core::Model::Video;

class TestJsonVideoParser : public QObject {
    Q_OBJECT

  private slots:
    static void testParse_data();
    static void testParse();

  public:
    static void testEqualityNoDate(const Video &lhs, const Video &rhs);
};

#endif