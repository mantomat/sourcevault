#ifndef TESTARTICLE_H
#define TESTARTICLE_H

#include <QObject>

class TestArticle : public QObject {
    Q_OBJECT

  private slots:

    static void testArticleUrlValidator_data();
    static void testArticleUrlValidator();
    static void testArticleUrl();

    static void testSourceNameValidator_data();
    static void testSourceNameValidator();
    static void testSourceName();

    static void testReadTimeMinutes();

    static void testPublicationDateValidator_data();
    static void testPublicationDateValidator();
    static void testPublicationDate();
};

#endif
