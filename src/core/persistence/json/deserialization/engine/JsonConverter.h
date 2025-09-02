#ifndef JSONCONVERTER_H
#define JSONCONVERTER_H

#include "JsonDeserializationError.h"
#include "model/MediumUserData.h"
#include "persistence/MediaSerializationConfigs.h"

#include <QJsonObject>

using Core::Model::MediumUserData;

namespace Core::Persistence::Json {

/**
 * Template which will be specialized to convert a simple Json value to a desired type.
 * These only check for `WrongMediumFieldType` errors, not for `SemanticValidationFailed`
 * errors.
 */
template <typename T> struct JsonConverter;

template <> struct JsonConverter<int> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs & /*unused*/)
        -> std::variant<JsonDeserializationError, int> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isDouble()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be an integer, but it's not a numeric type."}.arg(
                        key),
            };
        }

        const double value{mediumObject.value(key).toDouble()};
        if (value != static_cast<double>(static_cast<long long>(value)) || value > INT_MAX ||
            value < INT_MIN) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message = QString{"Field '%1' should be an integer, but it's a double"}.arg(key),
            };
        }

        return static_cast<int>(value);
    }
};

template <> struct JsonConverter<unsigned int> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs & /*unused*/)
        -> std::variant<JsonDeserializationError, unsigned int> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isDouble()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message = QString{"Field '%1' should be an unsigned integer, but it's not a "
                                   "numeric type."}
                               .arg(key),
            };
        }

        const double value{mediumObject.value(key).toDouble()};
        if (value != static_cast<double>(static_cast<long long>(value)) || value < 0 ||
            value > UINT_MAX) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be an unsigned integer, but it's not"}.arg(key),
            };
        }
        return static_cast<unsigned int>(value);
    }
};

template <> struct JsonConverter<QUrl> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs & /*unused*/)
        -> std::variant<JsonDeserializationError, QUrl> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isString()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be a URL string, but it is not even a string."}.arg(
                        key),
            };
        }
        QUrl url{mediumObject.value(key).toString(), QUrl::ParsingMode::StrictMode};
        if (url.isEmpty() || !url.isValid()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be a URL string, but it is an invalid URL."}.arg(
                        key),
            };
        }
        return url;
    }
};

template <> struct JsonConverter<QString> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs & /*unused*/)
        -> std::variant<JsonDeserializationError, QString> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isString()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message = QString{"Field '%1' should be a string, but it is not."}.arg(key),
            };
        }
        return mediumObject.value(key).toString();
    }
};

template <> struct JsonConverter<QDate> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs &configs)
        -> std::variant<JsonDeserializationError, QDate> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isString()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be a date string, but it is not even a string."}.arg(
                        key),
            };
        }
        auto date{QDate::fromString(mediumObject.value(key).toString(), configs.dateFormat)};
        if (date.isNull() || !date.isValid()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be a date string, but it is an invalid date."}.arg(
                        key),
            };
        }
        return date;
    }
};

template <> struct JsonConverter<MediumUserData::PriorityLevel> {
    static auto fromJson(const QJsonObject &mediumObject, const QString &key,
                         const MediaSerializationConfigs & /*unused*/)
        -> std::variant<JsonDeserializationError, MediumUserData::PriorityLevel> {
        assert(mediumObject.contains(key));

        if (!mediumObject.value(key).isDouble()) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be a date string, but it is not even a string."}.arg(
                        key),
            };
        }
        const double value{mediumObject.value(key).toDouble()};
        if (value != static_cast<double>(static_cast<long long>(value)) ||
            value < static_cast<double>(MediumUserData::PriorityLevel::min) ||
            value > static_cast<double>(MediumUserData::PriorityLevel::max)) {
            return JsonDeserializationError{
                .code = JsonDeserializationError::Code::WrongMediumFieldType,
                .errorLocation = mediumObject,
                .message =
                    QString{"Field '%1' should be an integer from '%2' to '%3', but it's not"}
                        .arg(key)
                        .arg(static_cast<int>(MediumUserData::PriorityLevel::min))
                        .arg(static_cast<int>(MediumUserData::PriorityLevel::max)),
            };
        }
        return static_cast<MediumUserData::PriorityLevel>(static_cast<uint8_t>(value));
    }
};

}

#endif