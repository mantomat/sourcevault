#ifndef DESERIALIZATIONERROR_H
#define DESERIALIZATIONERROR_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <variant>

namespace Core::Persistence::Json {

struct JsonDeserializationError {
    enum class Code : uint8_t {
        InvalidJson,
        InvalidRootType,
        MissingVersion,
        UnknownVersion,
        MissingMediaArray,
        TooManyFields,
        NonObjectMediaEntry,
        MissingMediumType,
        UnknownMediumType,
        MissingRequiredField,
        WrongMediumFieldType,
        SemanticValidationFailed
    };

    Code code;
    std::variant<QJsonDocument, QJsonArray, QJsonObject> errorLocation;
    QString message;
};

}

#endif