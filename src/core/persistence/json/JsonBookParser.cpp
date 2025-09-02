#include "JsonBookParser.h"

#include "model/Book.h"

using Core::Model::Book;

namespace Core::Persistence::Json {

JsonBookParser::JsonBookParser(MediaSerializationConfigs newConfigs)
    : configs{std::move(newConfigs)} {}

auto JsonBookParser::parse(const QJsonObject &bookObject, const QString &version) const
    -> std::variant<DeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    QString title{bookObject.value("title").toString()};
    const QUuid id{bookObject.value("id").toString()};
    std::unique_ptr<Book> book{Book::make(std::move(title), id, QDate::currentDate()).value()};

    auto commonFieldsResult{deserializeCommonFields<Book>(std::move(book), bookObject, version)};
    if (const auto *error{std::get_if<DeserializationError>(&commonFieldsResult)};
        error != nullptr) {
        return *error;
    }

    std::variant<DeserializationError, std::unique_ptr<Book>> result =
        std::move(std::get<std::unique_ptr<Book>>(commonFieldsResult));

    result = andThen<Book>(std::move(result),
                           optionalValidatedFieldParser<QString, Book>(
                               bookObject, "isbn", [](Book &b) -> auto & { return b.isbn(); }));
    result = andThen<Book>(std::move(result), optionalValidatedFieldParser<QString, Book>(
                                                  bookObject, "edition",
                                                  [](Book &b) -> auto & { return b.edition(); }));
    result = andThen<Book>(std::move(result), optionalValidatedFieldParser<QString, Book>(
                                                  bookObject, "publisher",
                                                  [](Book &b) -> auto & { return b.publisher(); }));
    result =
        andThen<Book>(std::move(result), optionalValidatedFieldParser<int, Book>(
                                             bookObject, "yearPublished",
                                             [](Book &b) -> auto & { return b.yearPublished(); }));
    result = andThen<Book>(std::move(result), optionalValidatedFieldParser<unsigned int, Book>(
                                                  bookObject, "pageNumber", [](Book &b) -> auto & {
                                                      return b.pageNumber();
                                                  }));
    result = andThen<Book>(std::move(result), optionalValidatedFieldParser<QString, Book>(
                                                  bookObject, "description", [](Book &b) -> auto & {
                                                      return b.description();
                                                  }));
    result =
        andThen<Book>(std::move(result), optionalValidatedFieldParser<QUrl, Book>(
                                             bookObject, "thumbnailUrl",
                                             [](Book &b) -> auto & { return b.thumbnailUrl(); }));

    if (auto *success = std::get_if<std::unique_ptr<Book>>(&result)) {
        return std::move(*success);
    }
    return std::get<DeserializationError>(result);
}

}