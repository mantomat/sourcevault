#ifndef TESTDESERIALIZECOMMONFIELDS_H
#define TESTDESERIALIZECOMMONFIELDS_H

#include <QObject>

class TestJsonParsingLib : public QObject {
    Q_OBJECT

  private slots:
    // Here we are only testing the core logic of `deserializeCommonFields`. The other templates are
    // just utilities widely used by concrete parsers (e.g. JsonBookParser, JsonArticleParser, ...),
    // and tested there.

    static void testDeserializeCommonFields_data();
    static void testDeserializeCommonFields();
};

#endif