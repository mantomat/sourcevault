#include "JsonMediumSerializerVisitor.h"

#include "model/MediumUserData.h"
#include "persistence/MediaSerializationConfigs.h"

#include <utility>

using Core::Model::MediumUserData;

namespace Core::Persistence::Json {

JsonMediumSerializerVisitor::JsonMediumSerializerVisitor(MediaSerializationConfigs newConfigs)
    : configs{std::move(newConfigs)} {}

void JsonMediumSerializerVisitor::visit(const Book &book) {
    serializedMedium = QJsonObject{};
    serializeBaseAttributes(book);

    serializeValidatedField<QString, QString>("isbn", book.isbn());
    serializeValidatedField<QString, QString>("edition", book.edition());
    serializeValidatedField<QString, QString>("publisher", book.publisher());
    serializeValidatedField<int, int>("yearPublished", book.yearPublished());
    serializeValidatedField<unsigned int, int>("pageNumber", book.pageNumber());
    serializeValidatedField<QString, QString>("description", book.description());
    serializeThumbnailUrl("thumbnailUrl", book.thumbnailUrl());
}

void JsonMediumSerializerVisitor::visit(const Article &article) {
    serializedMedium = QJsonObject{};
    serializeBaseAttributes(article);

    serializeValidatedField<QUrl, QString>("articleUrl", article.articleUrl(),
                                           urlToStringConverter);
    serializeValidatedField<QString, QString>("sourceName", article.sourceName());
    serializeValidatedField<unsigned int, int>("readTimeMinutes", article.readTimeMinutes());
    serializeValidatedField<QDate, QString>("publicationDate", article.publicationDate(),
                                            dateToStringConverter);
}

void JsonMediumSerializerVisitor::visit(const Video &video) {
    serializedMedium = QJsonObject{};
    serializeBaseAttributes(video);

    serializeValidatedField<QUrl, QString>("videoUrl", video.videoUrl(), urlToStringConverter);
    serializeValidatedField<unsigned int, int>("durationSeconds", video.durationSeconds());
    serializeValidatedField<QDate, QString>("uploadDate", video.uploadDate(),
                                            dateToStringConverter);
    serializeThumbnailUrl("thumbnailUrl", video.thumbnailUrl());
}

auto JsonMediumSerializerVisitor::getSerializedMedium() const -> QJsonObject {
    return serializedMedium;
}

void JsonMediumSerializerVisitor::serializeBaseAttributes(const Medium &medium) {
    serializedMedium["id"] = medium.id().toString(QUuid::StringFormat::WithoutBraces);
    // We ignore dateAdded since it's overwritten after each import.
    serializedMedium["title"] = medium.title();
    serializedMedium["favorite"] = medium.userData().favorite();

    serializeValidatedSet<QString, QString>("authors", medium.authors());
    serializeValidatedField<QString, QString>("language", medium.language());
    serializeValidatedSet<QString, QString>("topics", medium.userData().topics());
    serializeValidatedField<QString, QString>("notes", medium.userData().notes());
    serializeValidatedField<MediumUserData::PriorityLevel, int>(
        "priority", medium.userData().priority(), [](const MediumUserData::PriorityLevel &field) {
            return static_cast<std::underlying_type_t<MediumUserData::PriorityLevel>>(field);
        });
}

void JsonMediumSerializerVisitor::serializeThumbnailUrl(const QString &fieldName,
                                                        const ValidatedField<QUrl> &url) {
    if (!url.has() || url.get().value().isLocalFile()) {
        return;
    }
    serializedMedium[fieldName] = url.get().value().toString();
}

}