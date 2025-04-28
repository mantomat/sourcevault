#include "TestBook.h"

#include "model/Book.h"

#include <QTest>

using Core::Model::Book;

void TestBook::testIsbnValidator_data() {
    QTest::addColumn<QString>("candidateIsbn");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty ISBN should be invalid") << "" << false;
    QTest::addRow("Invalid ISBN should be invalid") << "123" << false;
    QTest::addRow("Valid ISBN-10 should be valid") << "0-306-40615-2" << true;
    QTest::addRow("Valid ISBN-13 should be valid") << "978-3-16-148410-0" << true;
}
void TestBook::testIsbnValidator() {
    QFETCH(QString, candidateIsbn);
    QFETCH(bool, expected);
    QCOMPARE(Book::isbnValidator(candidateIsbn), expected);
}
void TestBook::testIsbn() {
    Book book;
    const QString validIsbn{"978-3-16-148410-0"};
    book.isbn().set(validIsbn);
    const Book constBookValidIsbn{book};
    QCOMPARE(constBookValidIsbn.isbn().get(), std::make_optional(validIsbn));

    const QString invalidIsbn{};
    book.isbn().set(invalidIsbn);
    const Book constBookInvalidIsbn{book};
    QCOMPARE(constBookInvalidIsbn.isbn().get(), std::nullopt);
}

void TestBook::testEditionValidator_data() {
    QTest::addColumn<QString>("candidateEdition");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty edition should be invalid") << "" << false;
    QTest::addRow("Non-empty edition should be valid") << "First Edition" << true;
}
void TestBook::testEditionValidator() {
    QFETCH(QString, candidateEdition);
    QFETCH(bool, expected);
    QCOMPARE(Book::editionValidator(candidateEdition), expected);
}
void TestBook::testEdition() {
    Book book;
    const QString validEdition{"Second Edition"};
    book.edition().set(validEdition);
    const Book constBookValidEdition{book};
    QCOMPARE(constBookValidEdition.edition().get(), std::make_optional(validEdition));

    const QString invalidEdition{};
    book.edition().set(invalidEdition);
    const Book constBookInvalidEdition{book};
    QCOMPARE(constBookInvalidEdition.edition().get(), std::nullopt);
}

void TestBook::testPublisherValidator_data() {
    QTest::addColumn<QString>("candidatePublisher");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty publisher should be invalid") << "" << false;
    QTest::addRow("Non-empty publisher should be valid") << "O'Reilly" << true;
}

void TestBook::testPublisherValidator() {
    QFETCH(QString, candidatePublisher);
    QFETCH(bool, expected);
    QCOMPARE(Book::publisherValidator(candidatePublisher), expected);
}

void TestBook::testPublisher() {
    Book book;
    const QString validPublisher{"O'Reilly"};
    book.publisher().set(validPublisher);
    const Book constBookValidPublisher{book};
    QCOMPARE(constBookValidPublisher.publisher().get(), std::make_optional(validPublisher));

    const QString invalidPublisher{};
    book.publisher().set(invalidPublisher);
    const Book constBookInvalidPublisher{book};
    QCOMPARE(constBookInvalidPublisher.publisher().get(), std::nullopt);
}

void TestBook::testYearPublished() {
    Book book;
    constexpr int validYear{2023};
    book.yearPublished().set(validYear);
    const Book constBookValidYear{book};
    QCOMPARE(constBookValidYear.yearPublished().get(), std::make_optional(validYear));

    book.yearPublished().unset();
    const Book constBookInvalidYear{book};
    QCOMPARE(constBookInvalidYear.yearPublished().get(), std::nullopt);
}

void TestBook::testPageNumber() {
    Book book;
    constexpr unsigned int validPages{321};
    book.pageNumber().set(validPages);
    const Book constBookValidPages{book};
    QCOMPARE(constBookValidPages.pageNumber().get(), std::make_optional(validPages));

    book.pageNumber().unset();
    const Book constBookInvalidPages{book};
    QCOMPARE(constBookInvalidPages.pageNumber().get(), std::nullopt);
}

void TestBook::testDescriptionValidator_data() {
    QTest::addColumn<QString>("candidateDescription");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty description should be invalid") << "" << false;
    QTest::addRow("Non-empty description should be valid")
        << "A great book about software design." << true;
}
void TestBook::testDescriptionValidator() {
    QFETCH(QString, candidateDescription);
    QFETCH(bool, expected);
    QCOMPARE(Book::descriptionValidator(candidateDescription), expected);
}
void TestBook::testDescription() {
    Book book;
    const QString validDescription{"An excellent introduction to C++."};
    book.description().set(validDescription);
    const Book constBookValidDescription{book};
    QCOMPARE(constBookValidDescription.description().get(), std::make_optional(validDescription));

    const QString invalidDescription{};
    book.description().set(invalidDescription);
    const Book constBookInvalidDescription{book};
    QCOMPARE(constBookInvalidDescription.description().get(), std::nullopt);
}

void TestBook::testThumbnailUrlValidator_data() {
    QTest::addColumn<QUrl>("candidateUrl");
    QTest::addColumn<bool>("expected");

    QTest::addRow("Empty url should be invalid") << QUrl{} << false;
    QTest::addRow("Non-image url should be invalid")
        << QUrl{"https://www.example.com/book"} << false;
    QTest::addRow("Image url should be valid") << QUrl{"https://example.com/image.jpg"} << true;
    QTest::addRow("Any image url should be valid")
        << QUrl{"https://upload.wikimedia.org/wikipedia/commons/4/47/"
                "PNG_transparency_demonstration_1.png"}
        << true;
    QTest::addRow("Local image url should be valid")
        << QUrl::fromLocalFile("/Users/example/Pictures/cover.png") << true;
}
void TestBook::testThumbnailUrlValidator() {
    QFETCH(QUrl, candidateUrl);
    QFETCH(bool, expected);
    QCOMPARE(Book::thumbnailUrlValidator(candidateUrl), expected);
}
void TestBook::testThumbnailUrl() {
    Book book;
    const QUrl validThumbnailUrl{"https://example.com/image.jpg"};
    book.thumbnailUrl().set(validThumbnailUrl);
    const Book constBookValidThumbnailUrl{book};
    QCOMPARE(constBookValidThumbnailUrl.thumbnailUrl().get(),
             std::make_optional(validThumbnailUrl));

    const QUrl invalidThumbnailUrl{};
    book.thumbnailUrl().set(invalidThumbnailUrl);
    const Book constBookInvalidThumbnailUrl{book};
    QCOMPARE(constBookInvalidThumbnailUrl.thumbnailUrl().get(), std::nullopt);
}