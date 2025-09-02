#include "JsonMediaSerializer.h"

#include <QJsonArray>
#include <QJsonDocument>

namespace Core::Persistence::Json {

JsonMediaSerializer::JsonMediaSerializer(MediaSerializationConfigs newConfigs)
    : mediumSerializer{std::make_unique<JsonMediumSerializerVisitor>(std::move(newConfigs))} {}

auto JsonMediaSerializer::serialize(const std::vector<const Medium *> &media) const
    -> QJsonDocument {
    QJsonArray mediaJsonArray;

    std::ranges::for_each(media, [this, &mediaJsonArray](const Medium *m) {
        if (m == nullptr) {
            return;
        }
        m->accept(*mediumSerializer);
        mediaJsonArray.append(mediumSerializer->getSerializedMedium());
    });

    QJsonObject root;
    if (!mediaJsonArray.isEmpty()) {
        root["media"] = mediaJsonArray;
    }
    return QJsonDocument{root};
}

}
