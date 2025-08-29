#include "TestSearchScoreVisitor.h"

#include "model/Medium.h"
#include "queries/search/FieldScoreCalculator.h"
#include "queries/search/SearchScoreVisitor.h"

#include <QTest>

using Core::Model::Medium;

using Core::Queries::Search::FieldWeightLevels;
using Core::Queries::Search::SearchScoreVisitor;

using MediaGenerator = std::function<std::unique_ptr<Medium>()>;

void TestSearchScoreVisitor::testGetScore_data() {
    QTest::addColumn<SearchScoreVisitor>("visitor");
    QTest::addColumn<MediaGenerator>("mediumGenerator");
    QTest::addColumn<double>("expectedScore");

    auto book{Book::create("Blindness").value()};
    book.isbn().set("978-0156007757");
    book.authors().add("Jose Saramago");
    book.pageNumber().set(352);
    book.publisher().set("Mariner Books");
    book.description().set(
        "A city is hit by an epidemic of \"white blindness\""
        " which spares no one. Authorities confine the blind to an empty mental hospital, but "
        "there the criminal element holds everyone captive, stealing food rations and raping "
        "women. There is one eyewitness to this nightmare who guides seven strangers—among them a "
        "boy with no mother, a girl with dark glasses, a dog of tears—through the barren streets, "
        "and the procession becomes as uncanny as the surroundings are harrowing. A magnificent "
        "parable of loss and disorientation, Blindness has swept the reading public with its "
        "powerful portrayal of our worst appetites and weaknesses—and humanity's ultimately "
        "exhilarating spirit.");
    book.language().set("English");

    const MediaGenerator bookGenerator{[book]() { return std::make_unique<Book>(book); }};

    const FieldWeightLevels levels;

    QTest::addRow("Title exact match")
        << visitorGenerator("blindness") << bookGenerator << levels.identifiers;
    QTest::addRow("Isbn exact match")
        << visitorGenerator("978-0156007757") << bookGenerator << levels.identifiers;
    QTest::addRow("Author exact match")
        << visitorGenerator("Jose Saramago") << bookGenerator << levels.fields;

    QTest::addRow("Complete mismatch") << visitorGenerator("zzz") << MediaGenerator{[] {
        return std::make_unique<Book>(Book::create("aaa").value());
    }} << 0.0;
}
void TestSearchScoreVisitor::testGetScore() {
    QFETCH(SearchScoreVisitor, visitor);
    QFETCH(MediaGenerator, mediumGenerator);
    QFETCH(double, expectedScore);

    const std::unique_ptr<const Medium> medium{mediumGenerator()};
    medium->accept(visitor);

    QCOMPARE(visitor.getScore(), expectedScore);
}

void TestSearchScoreVisitor::testVisitBook_data() {
    QTest::addColumn<Book>("book");
    QTest::addColumn<SearchScoreVisitor>("visitor");
    QTest::addColumn<double>("expectedScore");

    const QString title{"title"};
    const QString isbn{"978-0-439-02348-1"};
    const QString author1{"author1"};
    const QString author2{"author2"};
    const QString description{"desc"};

    auto book{Book::create(title).value()};
    book.isbn().set(isbn);
    book.authors().add(author1);
    book.authors().add(author2);
    book.description().set(description);

    FieldWeightLevels weights{};

    QTest::addRow("Title should have max score and count as identifier")
        << book << visitorGenerator(title) << weights.identifiers;
    QTest::addRow("Isbn should have max score and count as identifier")
        << book << visitorGenerator(isbn) << weights.identifiers;
    QTest::addRow("One of the authors should have max score and count as field")
        << book << visitorGenerator(author2) << weights.fields;
    QTest::addRow("Description should have max score and count as field")
        << book << visitorGenerator(description) << weights.fields;
}

void TestSearchScoreVisitor::testVisitBook() {
    QFETCH(Book, book);
    QFETCH(SearchScoreVisitor, visitor);
    QFETCH(double, expectedScore);

    book.accept(visitor);

    QCOMPARE(visitor.getScore(), expectedScore);
}

void TestSearchScoreVisitor::testVisitArticle_data() {
    QTest::addColumn<Article>("article");
    QTest::addColumn<SearchScoreVisitor>("visitor");
    QTest::addColumn<double>("expectedScore");

    const QString title{"title"};
    const QString url{"https://www.nature.com/articles/d41586-025-02741-1"};
    const QString author1{"author1"};
    const QString author2{"author2"};
    const QString sourceName{"Nature"};

    auto article{Article::create(title).value()};
    article.articleUrl().set(url);
    article.authors().add(author1);
    article.authors().add(author2);
    article.sourceName().set(sourceName);

    FieldWeightLevels weights{};

    QTest::addRow("Title should have max score and count as identifier")
        << article << visitorGenerator(title) << weights.identifiers;
    QTest::addRow("Artice URL should have max score and count as identifier")
        << article << visitorGenerator(url) << weights.identifiers;
    QTest::addRow("One of the authors should have max score and count as field")
        << article << visitorGenerator(author2) << weights.fields;
    QTest::addRow("Source name should have max score and count as field")
        << article << visitorGenerator(sourceName) << weights.fields;
}
void TestSearchScoreVisitor::testVisitArticle() {
    QFETCH(Article, article);
    QFETCH(SearchScoreVisitor, visitor);
    QFETCH(double, expectedScore);

    article.accept(visitor);

    QCOMPARE(visitor.getScore(), expectedScore);
}

void TestSearchScoreVisitor::testVisitVideo_data() {
    QTest::addColumn<Video>("video");
    QTest::addColumn<SearchScoreVisitor>("visitor");
    QTest::addColumn<double>("expectedScore");

    const QString title{"title"};
    const QString url{"https://www.youtube.com/watch?v=dQw4w9WgXcQ"};
    const QString author1{"Rick Astley"};
    const QString author2{"Google LLC :)"};
    const QDate uploadDate{2020, 1, 1};

    auto video{Video::create(title).value()};
    video.videoUrl().set(url);
    video.authors().add(author1);
    video.authors().add(author2);
    video.uploadDate().set(uploadDate);

    FieldWeightLevels weights{};

    QTest::addRow("Title should have max score and count as identifier")
        << video << visitorGenerator(title) << weights.identifiers;
    QTest::addRow("Video URL should have max score and count as identifier")
        << video << visitorGenerator(url) << weights.identifiers;
    QTest::addRow("One of the authors should have max score and count as field")
        << video << visitorGenerator(author2) << weights.fields;
    QTest::addRow("Upload date should have max score and count as field")
        << video << visitorGenerator(uploadDate.toString(SearchOptions{}.dateFormat))
        << weights.fields;
}
void TestSearchScoreVisitor::testVisitVideo() {
    QFETCH(Video, video);
    QFETCH(SearchScoreVisitor, visitor);
    QFETCH(double, expectedScore);

    video.accept(visitor);

    QCOMPARE(visitor.getScore(), expectedScore);
}