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
        EmptyJson,
        InvalidRootType,
        MissingVersion,
        UnknownVersion,
        MissingMediaArray,
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