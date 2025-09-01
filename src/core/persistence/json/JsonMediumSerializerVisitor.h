#ifndef JSONMEDIUMSERIALIZERVISITOR_H
#define JSONMEDIUMSERIALIZERVISITOR_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Medium.h"
#include "model/ValidatedField.h"
#include "model/ValidatedSet.h"
#include "model/Video.h"
#include "persistence/MediaSerializationConfigs.h"
#include "shared/MediumVisitor.h"

#include <QJsonArray>
#include <QJsonObject>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Medium;
using Core::Model::ValidatedField;
using Core::Model::ValidatedSet;
using Core::Model::Video;
using Core::Persistence::MediaSerializationConfigs;
using Core::Shared::MediumVisitor;

namespace Core::Persistence::Json {

class JsonMediumSerializerVisitor : public MediumVisitor {

    QJsonObject serializedMedium;
    MediaSerializationConfigs configs;

  public:
    ~JsonMediumSerializerVisitor() override = default;
    JsonMediumSerializerVisitor(const JsonMediumSerializerVisitor &) = default;
    JsonMediumSerializerVisitor(JsonMediumSerializerVisitor &&) = default;
    auto operator=(const JsonMediumSerializerVisitor &) -> JsonMediumSerializerVisitor & = default;
    auto operator=(JsonMediumSerializerVisitor &&) -> JsonMediumSerializerVisitor & = default;

    explicit JsonMediumSerializerVisitor(
        MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    void visit(const Book &book) override;
    void visit(const Article &article) override;
    void visit(const Video &video) override;

    [[nodiscard]] auto getSerializedMedium() const -> QJsonObject;

  private:
    void serializeBaseAttributes(const Medium &medium);

    template <typename FieldType, typename JsonFieldType>
    void serializeValidatedField(
        const QString &fieldName, const ValidatedField<FieldType> &field,
        const std::function<JsonFieldType(const FieldType &)> &converter =
            [](const FieldType &element) -> JsonFieldType {
            return static_cast<JsonFieldType>(element);
        }) {
        if (!field.has()) {
            return;
        }
        serializedMedium[fieldName] = converter(field.get().value());
    }

    template <typename ElementsType, typename JsonElementsType>
    void serializeValidatedSet(
        const QString &fieldName, const ValidatedSet<ElementsType> &set,
        const std::function<JsonElementsType(const ElementsType &)> &converter =
            [](const ElementsType &element) { return static_cast<JsonElementsType>(element); }) {
        if (!set.has()) {
            return;
        }
        QJsonArray elements;
        std::ranges::for_each(set.get().value(),
                              [&elements, &converter](const ElementsType &element) {
                                  elements.append(converter(element));
                              });
        serializedMedium[fieldName] = elements;
    }

    void serializeThumbnailUrl(const QString &fieldName, const ValidatedField<QUrl> &url);

    std::function<QString(const QDate &)> dateToStringConverter{
        [this](const QDate &element) { return element.toString(configs.dateFormat); }};
    std::function<QString(const QUrl &)> urlToStringConverter{
        [](const QUrl &element) { return element.toString(); }};
};

}

#endif