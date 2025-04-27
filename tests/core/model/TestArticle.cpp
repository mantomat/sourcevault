#include "TestArticle.h"

#include "model/Article.h"

#include <QTest>

using Core::Model::Article;

void TestArticle::testArticleUrlValidator_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty url should be invalid") << QUrl{} << false;
    QTest::addRow("Valid url should be valid") << QUrl{"https://example.com/article"} << true;
}

void TestArticle::testArticleUrlValidator() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, expected);

    QCOMPARE(Article::articleUrlValidator(candidateUrl), expected);
}

void TestArticle::testArticleUrl() {
    Article article;

    const QUrl validUrl{"https://example.com/article"};
    article.articleUrl().set(validUrl);
    const Article constArticleValid{article};
    QCOMPARE(constArticleValid.articleUrl().get(), std::make_optional(validUrl));

    const QUrl invalidUrl{};
    article.articleUrl().set(invalidUrl);
    const Article constArticleInvalid{article};
    QCOMPARE(constArticleInvalid.articleUrl().get(), std::nullopt);
}

void TestArticle::testSourceNameValidator_data() {
    QTest::addColumn<QString>("candidateSourceName");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty source name should be invalid") << "" << false;
    QTest::addRow("Valid source name should be valid") << "Nature" << true;
}

void TestArticle::testSourceNameValidator() {
    QFETCH(QString, candidateSourceName);
    QFETCH(bool, expected);

    QCOMPARE(Article::sourceNameValidator(candidateSourceName), expected);
}

void TestArticle::testSourceName() {
    Article article;

    const QString validSourceName{"Nature"};
    article.sourceName().set(validSourceName);
    const Article constArticleValid{article};
    QCOMPARE(constArticleValid.sourceName().get(), std::make_optional(validSourceName));

    const QString invalidSourceName{};
    article.sourceName().set(invalidSourceName);
    const Article constArticleInvalid{article};
    QCOMPARE(constArticleInvalid.sourceName().get(), std::nullopt);
}

void TestArticle::testReadTimeMinutes() {
    Article article;

    constexpr unsigned int validTime{15};
    article.readTimeMinutes().set(validTime);
    const Article constArticleValid{article};
    QCOMPARE(constArticleValid.readTimeMinutes().get(), std::make_optional(validTime));

    article.readTimeMinutes().unset();
    const Article constArticleUnset{article};
    QCOMPARE(constArticleUnset.readTimeMinutes().get(), std::nullopt);
}

void TestArticle::testPublicationDateValidator_data() {
    QTest::addColumn<QDate>("candidateDate");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Invalid date should be invalid") << QDate{} << false;
    QTest::addRow("Valid date should be valid") << QDate{2023, 10, 5} << true;
}

void TestArticle::testPublicationDateValidator() {
    QFETCH(QDate, candidateDate);
    QFETCH(bool, expected);

    QCOMPARE(Article::publicationDateValidator(candidateDate), expected);
}

void TestArticle::testPublicationDate() {
    Article article;

    const QDate validDate{2023, 10, 5};
    article.publicationDate().set(validDate);
    const Article constArticleValid{article};
    QCOMPARE(constArticleValid.publicationDate().get(), std::make_optional(validDate));

    constexpr QDate invalidDate{};
    article.publicationDate().set(invalidDate);
    const Article constArticleInvalid{article};
    QCOMPARE(constArticleInvalid.publicationDate().get(), std::nullopt);
}