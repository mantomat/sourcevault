#ifndef TESTARTICLE_H
#define TESTARTICLE_H

#include <QObject>

class TestArticle : public QObject {
    Q_OBJECT

  private slots:

    static void testCreate_data();
    static void testCreate();

    static void testArticleUrl_data();
    static void testArticleUrl();

    static void testSourceName_data();
    static void testSourceName();

    static void testReadTimeMinutes_data();
    static void testReadTimeMinutes();

    static void testPublicationDate_data();
    static void testPublicationDate();
};

#endif
