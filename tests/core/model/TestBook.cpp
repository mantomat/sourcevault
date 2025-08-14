#include "TestBook.h"

#include "TestMedium.h"
#include "TestValidatedField.h"
#include "model/Book.h"

#include <QTest>

using Core::Model::Book;

const auto bookBuilder{[] { return Book::create("default title").value(); }};

void TestBook::testCreate_data() {
    TestMedium::testCreateData();
}

void TestBook::testCreate() {
    QFETCH(bool, shouldBeValid);
    QFETCH(QString, candidateTitle);
    QFETCH(QUuid, candidateId);
    QFETCH(QDate, candidateDateAdded);

    const auto optionalBook{Book::create(candidateTitle, candidateId, candidateDateAdded)};
    if (shouldBeValid) {
        QVERIFY2(optionalBook.has_value(), "The book must be created");
        QCOMPARE(optionalBook.value().title(), candidateTitle);
        QCOMPARE(optionalBook.value().id(), candidateId);
        QCOMPARE(optionalBook.value().dateAdded(), candidateDateAdded);
    } else {
        QVERIFY2(!optionalBook.has_value(), "The book mustn't be created");
    }
}

void TestBook::testIsbn_data() {
    QTest::addColumn<QString>("candidateIsbn");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty ISBNs are invalid") << "" << false;
    QTest::addRow("Invalid ISBNs are invalid") << "123" << false;
    QTest::addRow("ISBN-10's are valid") << "0-306-40615-2" << true;
    QTest::addRow("ISBN-13's are valid") << "978-3-16-148410-0" << true;
}

void TestBook::testIsbn() {
    QFETCH(QString, candidateIsbn);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, QString>(
        bookBuilder, &Book::isbnValidator,
        [](Book& b) -> ValidatedField<QString>& { return b.isbn(); },
        [](const Book& b) -> const ValidatedField<QString>& { return b.isbn(); }, candidateIsbn,
        shouldBeValid);
}

void TestBook::testEdition_data() {
    QTest::addColumn<QString>("candidateEdition");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid editions") << "" << false;
    QTest::addRow("Space and CR-only strings are invalid editions") << "  \n " << false;
    QTest::addRow("Every non-empty string is a valid edition") << "First Edition" << true;
}

void TestBook::testEdition() {
    QFETCH(QString, candidateEdition);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, QString>(
        bookBuilder, &Book::editionValidator,
        [](Book& b) -> ValidatedField<QString>& { return b.edition(); },
        [](const Book& b) -> const ValidatedField<QString>& { return b.edition(); },
        candidateEdition, shouldBeValid);
}

void TestBook::testPublisher_data() {
    QTest::addColumn<QString>("candidatePublisher");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid publishers") << "" << false;
    QTest::addRow("Space and CR-only strings are invalid publishers") << "  \n " << false;
    QTest::addRow("Every non-empty string is a valid publisher") << "O'Reilly" << true;
}

void TestBook::testPublisher() {
    QFETCH(QString, candidatePublisher);
    QFETCH(bool, shouldBeValid);
    TestValidatedField::testValidatedFieldHelper<Book, QString>(
        bookBuilder, &Book::publisherValidator,
        [](Book& b) -> ValidatedField<QString>& { return b.publisher(); },
        [](const Book& b) -> const ValidatedField<QString>& { return b.publisher(); },
        candidatePublisher, shouldBeValid);
}

void TestBook::testYearPublished_data() {
    QTest::addColumn<int>("candidateYearPublished");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Books can't be published before 10000 BC") << -10001 << false;
    QTest::addRow("Books can be published from 10000 BC onward") << -10000 << true;
    QTest::addRow("Books can be published from 10000 BC onward") << 0 << true;
    QTest::addRow("Books can be published from 10000 BC onward") << 2100 << true;
}

void TestBook::testYearPublished() {
    QFETCH(int, candidateYearPublished);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, int>(
        bookBuilder, &Book::yearPublishedValidator,
        [](Book& b) -> ValidatedField<int>& { return b.yearPublished(); },
        [](const Book& b) -> const ValidatedField<int>& { return b.yearPublished(); },
        candidateYearPublished, shouldBeValid);
}

void TestBook::testPageNumber_data() {
    QTest::addColumn<unsigned int>("candidatePageNumber");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Zero pages is an invalid page number") << 0u << false;
    QTest::addRow("Any number of pages greater than zero is a valid page number") << 1u << true;
    QTest::addRow("Any number of pages greater than zero is a valid page number") << 1234u << true;
}

void TestBook::testPageNumber() {
    QFETCH(unsigned int, candidatePageNumber);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, unsigned int>(
        bookBuilder, &Book::pageNumberValidator,
        [](Book& b) -> ValidatedField<unsigned int>& { return b.pageNumber(); },
        [](const Book& b) -> const ValidatedField<unsigned int>& { return b.pageNumber(); },
        candidatePageNumber, shouldBeValid);
}

void TestBook::testDescription_data() {
    QTest::addColumn<QString>("candidateDescription");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty strings are invalid book descriptions") << "" << false;
    QTest::addRow("Space and CR-only strings are invalid book descriptions") << "  \n " << false;
    QTest::addRow("Every non-empty string is a valid book description")
        << "A great book about software design." << true;
}
void TestBook::testDescription() {
    QFETCH(QString, candidateDescription);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, QString>(
        bookBuilder, &Book::descriptionValidator,
        [](Book& b) -> ValidatedField<QString>& { return b.description(); },
        [](const Book& b) -> const ValidatedField<QString>& { return b.description(); },
        candidateDescription, shouldBeValid);
}

void TestBook::testThumbnailUrl_data() {
    QTest::addColumn<QUrl>("candidateThumbnailUrl");
    QTest::addColumn<bool>("shouldBeValid");

    QTest::addRow("Empty urls are invalid") << QUrl{} << false;

    QTest::addRow("Only local paths, http and https - i.e. mailto is not")
        << QUrl{"mailto:user@example.com"} << false;
    QTest::addRow("Only local paths, http and https - i.e. ftp is not")
        << QUrl{"ftp://user@host/foo/image.png"} << false;

    QTest::addRow("Non-image http urls are invalid")
        << QUrl{"http://www.example.com/book"} << false;
    QTest::addRow("Non-image https urls are invalid")
        << QUrl{"https://www.example.com/book"} << false;
    QTest::addRow("Non-image file urls are invalid")
        << QUrl{"file://www.example.com/book"} << false;
    QTest::addRow("Non-image local paths are invalid")
        << QUrl::fromLocalFile("/etc/passwd") << false;

    QTest::addRow(".jpg image http urls are valid") << QUrl{"http://example.com/image.jpg"} << true;
    QTest::addRow(".jpeg image http urls are valid")
        << QUrl{"http://example.com/image.jpeg"} << true;
    QTest::addRow(".png image http urls are valid")
        << QUrl{"http://upload.wikimedia.org/wikipedia/commons/4/47/"
                "PNG_transparency_demonstration_1.png"}
        << true;

    QTest::addRow(".jpg image https urls are valid")
        << QUrl{"https://example.com/image.jpg"} << true;
    QTest::addRow(".jpeg image https urls are valid")
        << QUrl{"https://example.com/image.jpeg"} << true;
    QTest::addRow(".png image https urls are valid")
        << QUrl{"https://upload.wikimedia.org/wikipedia/commons/4/47/"
                "PNG_transparency_demonstration_1.png"}
        << true;

    QTest::addRow(".jpg local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.jpg") << true;
    QTest::addRow(".jpeg local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.jpeg") << true;
    QTest::addRow(".png local image urls are valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.png") << true;
}

void TestBook::testThumbnailUrl() {
    QFETCH(QUrl, candidateThumbnailUrl);
    QFETCH(bool, shouldBeValid);

    TestValidatedField::testValidatedFieldHelper<Book, QUrl>(
        bookBuilder, &Book::thumbnailUrlValidator,
        [](Book& b) -> ValidatedField<QUrl>& { return b.thumbnailUrl(); },
        [](const Book& b) -> const ValidatedField<QUrl>& { return b.thumbnailUrl(); },
        candidateThumbnailUrl, shouldBeValid);
}