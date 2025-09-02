#ifndef JSONDESERIALIZATIONUTILS_H
#define JSONDESERIALIZATIONUTILS_H

#include "JsonConverter.h"
#include "JsonDeserializationError.h"
#include "model/Medium.h"
#include "model/MediumUserData.h"
#include "model/ValidatedField.h"
#include "model/ValidatedSet.h"
#include "persistence/MediaSerializationConfigs.h"
#include "shared/ConcreteMediumConcept.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using Core::Model::Medium;
using Core::Model::MediumUserData;
using Core::Model::ValidatedField;
using Core::Model::ValidatedSet;
using Core::Shared::ConcreteMedium;

namespace Core::Persistence::Json {

/**
 * A functional helper that takes a parsing result `previousResult`, and
 * - if it's an error, it just propagates the error.
 * - if it's a valid result, it applies the `nextStep` function to it to parse the next field.
 *   - if the parsing succeeds, it returns the final populated Medium
 *   - otherwise, it returns the newly-generated error.
 */
template <ConcreteMedium MediumType>
[[nodiscard]] auto
andThen(std::variant<JsonDeserializationError, std::unique_ptr<MediumType>> previousResult,
        std::function<std::optional<JsonDeserializationError>(MediumType &)> nextStep)
    -> std::variant<JsonDeserializationError, std::unique_ptr<MediumType>> {

    if (auto *error = std::get_if<JsonDeserializationError>(&previousResult)) {
        return *error;
    }
    auto mediumPtr = std::get<std::unique_ptr<MediumType>>(std::move(previousResult));

    if (std::optional<JsonDeserializationError> nextError = nextStep(*mediumPtr);
        nextError.has_value()) {
        return nextError.value();
    }
    return mediumPtr;
}

/**
 * A functional helper that returns the procedure (`step`) that must be applied
 * to a field in order to convert it to a ValidatedField of type `FieldType` and assign it to
 * the Medium.
 */
template <typename FieldType, ConcreteMedium MediumType>
[[nodiscard]] auto
optionalValidatedFieldParser(const QJsonObject &obj, const QString &key,
                             std::function<ValidatedField<FieldType> &(MediumType &)> getField,
                             const MediaSerializationConfigs &configs = MediaSerializationConfigs{})
    -> std::function<std::optional<JsonDeserializationError>(MediumType &)> {

    return [obj, key, getField,
            configs](MediumType &medium) -> std::optional<JsonDeserializationError> {
        if (!obj.contains(key)) {
            return std::nullopt;
        }

        const auto jsonValue = obj.value(key);

        auto conversionResult = JsonConverter<FieldType>::fromJson(obj, key, configs);
        if (const auto *error = std::get_if<JsonDeserializationError>(&conversionResult)) {
            return *error;
        }

        const auto &value = std::get<FieldType>(conversionResult);
        const bool wasSet{getField(medium).set(value)};
        if (!wasSet) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::SemanticValidationFailed,
                .errorLocation = obj,
                .message = QString{"Failed to validate field %1 semantically."}.arg(key),
            };
        }
        return std::nullopt;
    };
}

/**
 * A functional helper that returns the procedure (`step`) that must be applied
 * to a field in order to convert it to a ValidatedSet of strings and assign it to the
 * Medium.
 */
template <ConcreteMedium MediumType>
auto optionalValidatedSetParser(
    const QJsonObject &obj, const QString &key,
    std::function<ValidatedSet<QString> &(MediumType &)> getField,
    const MediaSerializationConfigs & /*unused*/ = MediaSerializationConfigs{})
    -> std::function<std::optional<JsonDeserializationError>(MediumType &)> {

    return [=](MediumType &medium) -> std::optional<JsonDeserializationError> {
        if (!obj.contains(key)) {
            return std::nullopt;
        }

        const auto jsonValue = obj.value(key);

        if (!jsonValue.isArray()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = obj,
                .message = QString("Field '%1' should be an array of strings.").arg(key)};
        }

        std::set<QString> values;
        for (const auto &item : jsonValue.toArray()) {
            if (!item.isString()) {
                return JsonDeserializationError{
                    .code = JsonDeserializationError::Code::WrongMediumFieldType,
                    .errorLocation = obj,
                    .message =
                        QString("An item in the array for field '%1' is not a string.").arg(key)};
            }
            values.insert(item.toString());
        }

        if (!getField(medium).set(values)) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::SemanticValidationFailed,
                .errorLocation = obj,
                .message = QString("Values for field '%1' failed semantic validation.").arg(key)};
        }

        return std::nullopt;
    };
}

template <ConcreteMedium MediumType>
[[nodiscard]] auto deserializeCommonFields(std::unique_ptr<MediumType> medium,
                                           const QJsonObject &mediumObject, const QString &version)
    -> std::variant<JsonDeserializationError, std::unique_ptr<MediumType>> {
    // We only manage this version for now.
    assert(version == "1.0");

    medium->userData().favorite() = mediumObject.value("favorite").toBool();

    std::variant<JsonDeserializationError, std::unique_ptr<MediumType>> result = std::move(medium);

    result = andThen(std::move(result), optionalValidatedSetParser<MediumType>(
                                            mediumObject, "authors",
                                            [](MediumType &m) -> auto & { return m.authors(); }));
    result = andThen(std::move(result), optionalValidatedFieldParser<QString, MediumType>(
                                            mediumObject, "language",
                                            [](MediumType &m) -> auto & { return m.language(); }));
    result = andThen(std::move(result), optionalValidatedSetParser<MediumType>(
                                            mediumObject, "topics", [](MediumType &m) -> auto & {
                                                return m.userData().topics();
                                            }));
    result = andThen(std::move(result), optionalValidatedFieldParser<QString, MediumType>(
                                            mediumObject, "notes", [](MediumType &m) -> auto & {
                                                return m.userData().notes();
                                            }));
    result = andThen(std::move(result),
                     optionalValidatedFieldParser<MediumUserData::PriorityLevel, MediumType>(
                         mediumObject, "priority",
                         [](MediumType &m) -> auto & { return m.userData().priority(); }));

    return result;
}

}

#endif