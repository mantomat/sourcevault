#ifndef JSONMEDIADESERIALIZER_H
#define JSONMEDIADESERIALIZER_H

#include "JsonMediumParser.h"
#include "model/Library.h"
#include "model/ValidatedField.h"
#include "model/ValidatedSet.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <unordered_map>

using Core::Model::Library;
using Core::Model::Medium;
using Core::Model::MediumUserData;
using Core::Model::ValidatedField;
using Core::Model::ValidatedSet;

namespace Core::Persistence::Json {

class JsonMediaDeserializer {

    std::unordered_map<QString, std::unique_ptr<JsonMediumParser>> parsers;

  public:
    explicit JsonMediaDeserializer(
        MediaSerializationConfigs newConfigs = MediaSerializationConfigs{});

    constexpr const static std::array<const char *, 1> supportedVersions{"1.0"};

    [[nodiscard]] auto deserialize(const QJsonDocument &document) const
        -> std::variant<JsonDeserializationError, Library>;

  private:
    [[nodiscard]] static auto preliminaryDocumentValidation(const QJsonDocument &document)
        -> std::optional<JsonDeserializationError>;

    [[nodiscard]] auto deserializeMedia(const QJsonArray &mediaArray, const QString &version) const
        -> std::variant<JsonDeserializationError, Library>;

    [[nodiscard]] auto deserializeMedium(const QJsonObject &mediumObject,
                                         const QString &version) const
        -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>>;
    [[nodiscard]] static auto preliminaryMediumValidation(const QJsonObject &mediumObject,
                                                          const QString &version)
        -> std::optional<JsonDeserializationError>;
};

}

#endif