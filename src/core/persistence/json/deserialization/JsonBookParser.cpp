#include "JsonBookParser.h"

#include "model/Book.h"

using Core::Model::Book;

namespace Core::Persistence::Json {

JsonBookParser::JsonBookParser(MediaSerializationConfigs newConfigs)
    : lib{std::move(newConfigs)} {}

auto JsonBookParser::parse(const QJsonObject &bookObject, const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    using Lib = JsonParsingLib<Book>;

    auto result{lib.deserializeCommonFields(bookObject, version)};

    result = Lib::andThen(std::move(result),
                          lib.optionalValidatedFieldParser<QString>(
                              bookObject, "isbn", [](Book &b) -> auto & { return b.isbn(); }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QString>(
                                                 bookObject, "edition",
                                                 [](Book &b) -> auto & { return b.edition(); }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QString>(
                                                 bookObject, "publisher",
                                                 [](Book &b) -> auto & { return b.publisher(); }));
    result = Lib::andThen(
        std::move(result),
        lib.optionalValidatedFieldParser<int>(bookObject, "yearPublished",
                                              [](Book &b) -> auto & { return b.yearPublished(); }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<unsigned int>(
                                                 bookObject, "pageNumber",
                                                 [](Book &b) -> auto & { return b.pageNumber(); }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QString>(
                                                 bookObject, "description", [](Book &b) -> auto & {
                                                     return b.description();
                                                 }));
    result = Lib::andThen(std::move(result), lib.optionalValidatedFieldParser<QUrl>(
                                                 bookObject, "thumbnailUrl", [](Book &b) -> auto & {
                                                     return b.thumbnailUrl();
                                                 }));

    if (auto *success = std::get_if<std::unique_ptr<Book>>(&result)) {
        return std::move(*success);
    }
    return std::get<JsonDeserializationError>(result);
}

}