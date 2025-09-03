#include "JsonMediaDeserializer.h"

#include "engine/JsonDeserializationUtils.h"
#include "model/Medium.h"
#include "parsers/JsonArticleParser.h"
#include "parsers/JsonBookParser.h"
#include "parsers/JsonVideoParser.h"

#include <algorithm>

using Core::Model::Medium;

namespace Core::Persistence::Json {

JsonMediaDeserializer::JsonMediaDeserializer(MediaSerializationConfigs newConfigs)
    : parsers{[&newConfigs] {
        std::unordered_map<QString, std::unique_ptr<JsonMediumParser>> newParsers;
        newParsers["article"] = std::make_unique<JsonArticleParser>(newConfigs);
        newParsers["book"] = std::make_unique<JsonBookParser>(newConfigs);
        newParsers["video"] = std::make_unique<JsonVideoParser>(std::move(newConfigs));
        return newParsers;
    }()} {}

auto JsonMediaDeserializer::deserialize(const QJsonDocument &document) const
    -> std::variant<JsonDeserializationError, Library> {
    const auto preliminaryErrorOpt{preliminaryDocumentValidation(document)};
    if (preliminaryErrorOpt.has_value()) {
        return preliminaryErrorOpt.value();
    }

    // Invariant: The document is sane, there is the `media` array and a valid version.
    return deserializeMedia(document.object().value("media").toArray(),
                            document.object().value("version").toString());
}

auto JsonMediaDeserializer::preliminaryDocumentValidation(const QJsonDocument &document)
    -> std::optional<JsonDeserializationError> {

    if (document.isNull()) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::InvalidJson,
                                        .errorLocation = document,
                                        .message = "Invalid or null Json document."};
    }

    if (!document.isObject()) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::InvalidRootType,
                                        .errorLocation = document,
                                        .message = "Root is not an object. It must be an object."};
    }

    if (!document.object().contains("version")) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::MissingVersion,
                                        .errorLocation = document.object(),
                                        .message = "Format version is not specified."};
    }

    if (const auto version{document.object().value("version")};
        !version.isString() || std::ranges::none_of(supportedVersions, [&version](const char *ver) {
            return QString{ver} == version.toString();
        })) {
        return JsonDeserializationError{
            .code = JsonDeserializationError::Code::UnknownVersion,
            .errorLocation = document.object(),
            .message = QString{"`version` is not a string or is not supported."}};
    }

    if (!document.object().contains("media") || !document.object().value("media").isArray()) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::MissingMediaArray,
                                        .errorLocation = document.object(),
                                        .message =
                                            QString{"`media` is not there or is not an array."}};
    }

    if (document.object().size() != 2) {
        return JsonDeserializationError{
            .code = JsonDeserializationError::Code::TooManyFields,
            .errorLocation = document.object(),
            .message = QString{
                "The root object has too many fields. Only `version` and `media` are allowed"}};
    }

    return std::nullopt;
}

auto JsonMediaDeserializer::deserializeMedia(const QJsonArray &mediaArray,
                                             const QString &version) const
    -> std::variant<JsonDeserializationError, Library> {

    // We only manage this version for now.
    assert(version == "1.0");

    Library lib;
    for (const auto &entry : mediaArray) {
        if (!entry.isObject()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::NonObjectMediaEntry,
                .errorLocation = mediaArray,
                .message = QString{"An entry of the `media` array is not an object."}};
        }

        auto deserializationResult{deserializeMedium(entry.toObject(), version)};
        if (const auto *error{std::get_if<JsonDeserializationError>(&deserializationResult)};
            error != nullptr) {
            return *error;
        }
        lib.addMedium(std::move(std::get<std::unique_ptr<const Medium>>(deserializationResult)));
    }

    return lib;
}

auto JsonMediaDeserializer::deserializeMedium(const QJsonObject &mediumObject,
                                              const QString &version) const
    -> std::variant<JsonDeserializationError, std::unique_ptr<const Medium>> {
    // We only manage this version for now.
    assert(version == "1.0");

    const auto validationResults{preliminaryMediumValidation(mediumObject, version)};
    if (validationResults.has_value()) {
        return validationResults.value();
    }

    const QString type{mediumObject.value("type").toString()};
    if (!parsers.contains(type)) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::UnknownMediumType,
                                        .errorLocation = mediumObject,
                                        .message =
                                            QString{"`Medium` object has an unknown type entry."}};
    }
    return parsers.at(type)->parse(mediumObject, version);
}

auto JsonMediaDeserializer::preliminaryMediumValidation(const QJsonObject &mediumObject,
                                                        const QString &version)
    -> std::optional<JsonDeserializationError> {
    // We only manage this version for now.
    assert(version == "1.0");

    if (!mediumObject.contains("type")) {
        return JsonDeserializationError{.code = JsonDeserializationError::Code::MissingMediumType,
                                        .errorLocation = mediumObject,
                                        .message =
                                            QString{"`Medium` object doesn't have a `type` entry"}};
    }

    if (!mediumObject.value("type").isString()) {
        return JsonDeserializationError{
            .code = JsonDeserializationError::Code::UnknownMediumType,
            .errorLocation = mediumObject,
            .message = QString{
                "`Medium` object has a `type` entry of the wrong type. It must be a string."}};
    }

    if (!mediumObject.contains("id") || !mediumObject.value("id").isString()) {
        return JsonDeserializationError{.code =
                                            JsonDeserializationError::Code::MissingRequiredField,
                                        .errorLocation = mediumObject,
                                        .message = QString{"`id` (string) is required."}};
    }

    if (const QUuid id{mediumObject.value("id").toString()};
        id.isNull() || !Medium::idValidator(id)) {
        return JsonDeserializationError{
            .code = JsonDeserializationError::Code::SemanticValidationFailed,
            .errorLocation = mediumObject,
            .message = QString{"Invalid value for `id` Medium attribute."}};
    }

    if (!mediumObject.contains("title") || !mediumObject.value("title").isString()) {
        return JsonDeserializationError{.code =
                                            JsonDeserializationError::Code::MissingRequiredField,
                                        .errorLocation = mediumObject,
                                        .message = QString{"`title` (string) is required."}};
    }

    if (const QString title{mediumObject.value("title").toString()};
        !Medium::titleValidator(title)) {
        return JsonDeserializationError{
            .code = JsonDeserializationError::Code::SemanticValidationFailed,
            .errorLocation = mediumObject,
            .message = QString{"Invalid value for `title` Medium attribute."}};
    }

    if (!mediumObject.contains("favorite") || !mediumObject.value("favorite").isBool()) {
        return JsonDeserializationError{.code =
                                            JsonDeserializationError::Code::MissingRequiredField,
                                        .errorLocation = mediumObject,
                                        .message = QString{"`favorite` (bool) is required."}};
    }

    return std::nullopt;
}

}