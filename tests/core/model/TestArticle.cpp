#include "TestArticle.h"

#include "TestMedium.h"
#include "TestValidatedField.h"
#include "model/Article.h"

#include <QTest>

using Core::Model::Article;

const auto articleBuilder{[] { return Article::create("default title").value(); }};

void TestArticle::testClone() {
    auto articleToClone{std::make_unique<Article>(articleBuilder())};
    articleToClone->authors().add("me");
    articleToClone->readTimeMinutes().set(1);
    articleToClone->userData().favorite() = false;

    auto clone{articleToClone->clone()};

    QVERIFY(dynamic_cast<Article*>(clone.get()) != nullptr);
    QVERIFY(articleToClone != clone);
    QCOMPARE(*articleToClone, *clone);
}

void TestArticle::testCreate_data() {
    TestMedium::testCreateData();
}

void TestArticle::testCreate() {
    QFETCH(bool, shouldBeValid);
    QFETCH(QString, candidateTitle);
    QFETCH(QUuid, candidateId);
    QFETCH(QDate, candidateDateAdded);

    const auto optionalArticle{Article::create(candidateTitle, candidateId, candidateDateAdded)};
    if (shouldBeValid) {
        QVERIFY2(optionalArticle.has_value(), "The article must be created");
        QCOMPARE(optionalArticle.value().title(), candidateTitle);
        QCOMPARE(optionalArticle.value().id(), candidateId);
        QCOMPARE(optionalArticle.value().dateAdded(), candidateDateAdded);
    } else {
        QVERIFY2(!optionalArticle.has_value(), "The article mustn't be created");
    }
}

void TestArticle::testArticleUrl_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty urls are invalid") << QUrl{} << false;
    QTest::addRow("Valid https urls are valid") << QUrl{"https://example.com/article"} << true;
    QTest::addRow("Valid http urls are valid") << QUrl{"http://example.com/article"} << true;
    QTest::addRow("Every url other than http/https is invalid - ftp")
        << QUrl{"ftp://user@host/foo/bar.txt"} << false;
    QTest::addRow("Every url other than http/https is invalid - mailto")
        << QUrl{"mailto:someone@example.com"} << false;
}

void TestArticle::testArticleUrl() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Article, QUrl>(
        articleBuilder, &Article::articleUrlValidator,
        [](Article& article) -> ValidatedField<QUrl>& { return article.articleUrl(); },
        [](const Article& article) -> const ValidatedField<QUrl>& { return article.articleUrl(); },
        candidateUrl, shouldBeValid);
}

void TestArticle::testSourceName_data() {
    QTest::addColumn<QString>("candidateSourceName");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid source names") << "" << false;
    QTest::addRow("Space and CR-only strings are invalid source names") << "  \n " << false;
    QTest::addRow("Non-empty strings are valid source names") << "Nature" << true;
}

void TestArticle::testSourceName() {
    QFETCH(QString, candidateSourceName);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Article, QString>(
        articleBuilder, &Article::sourceNameValidator,
        [](Article& article) -> ValidatedField<QString>& { return article.sourceName(); },
        [](const Article& article) -> const ValidatedField<QString>& {
            return article.sourceName();
        },
        candidateSourceName, shouldBeValid);
}

void TestArticle::testReadTimeMinutes_data() {
    QTest::addColumn<unsigned int>("candidateReadTimeMinutes");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Zero minutes is an invalid read time") << 0u << false;
    QTest::addRow("Any integer greater than zero is a valid read time") << 1u << true;
    QTest::addRow("Any integer greater than zero is a valid read time") << 100u << true;
}

void TestArticle::testReadTimeMinutes() {
    QFETCH(unsigned int, candidateReadTimeMinutes);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Article, unsigned int>(
        articleBuilder, &Article::readTimeMinutesValidator,
        [](Article& article) -> ValidatedField<unsigned int>& { return article.readTimeMinutes(); },
        [](const Article& article) -> const ValidatedField<unsigned int>& {
            return article.readTimeMinutes();
        },
        candidateReadTimeMinutes, shouldBeValid);
}

void TestArticle::testPublicationDate_data() {
    QTest::addColumn<QDate>("candidatePublicationDate");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("An empty date is an invalid publication date") << QDate{} << false;
    QTest::addRow("Every non-empty date is a valid publication date") << QDate{2023, 10, 5} << true;
}

void TestArticle::testPublicationDate() {
    QFETCH(QDate, candidatePublicationDate);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Article, QDate>(
        articleBuilder, &Article::publicationDateValidator,
        [](Article& article) -> ValidatedField<QDate>& { return article.publicationDate(); },
        [](const Article& article) -> const ValidatedField<QDate>& {
            return article.publicationDate();
        },
        candidatePublicationDate, shouldBeValid);
}