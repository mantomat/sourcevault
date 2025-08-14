#include "Library.h"

#include <algorithm>
#include <ranges>

namespace Core::Model {

std::vector<const Medium*> Library::getAllMedia() const {
    auto range = media | std::views::values |
                 std::views::transform([](const auto& ptr) { return ptr.get(); });
    return {range.begin(), range.end()};
}

std::set<QString> Library::getAllTopics() const {
    std::set<QString> topics;
    std::ranges::for_each(media, [&](const auto& pair) {
        const auto& [_, medium]{pair};
        topics.merge(medium->userData().topics().get().value_or(std::set<QString>{}));
    });
    return topics;
}

std::optional<const Medium*> Library::getMedium(const QUuid& id) const {
    if (!media.contains(id)) {
        return std::nullopt;
    }
    return std::make_optional(media.at(id).get());
}

size_t Library::mediaCount() const {
    return media.size();
}

void Library::setMedia(std::vector<std::unique_ptr<const Medium>> newMedia) {
    media.clear();
    for (auto& medium : newMedia) {
        if (medium == nullptr)
            continue;

        media.emplace(medium->id(), std::move(medium));
    }
    emit mediaChanged();
}

// The clangd warning on this line is a false positive.
// This is a sink function for a move-only type, so taking the
// unique_ptr by value is the correct and idiomatic approach.
// NOLINTNEXTLINE(performance-unnecessary-value-param)
void Library::merge(std::unique_ptr<Library> other) {
    if (other == nullptr) {
        return;
    }

    const auto countBeforeMerge{mediaCount()};
    media.merge(std::move(other->media));

    if (const auto countAfterMerge{mediaCount()}; countBeforeMerge != countAfterMerge)
        emit mediaChanged();
}

bool Library::addMedium(std::unique_ptr<const Medium> newMedium) {
    if (newMedium == nullptr)
        return false;

    const auto [_, success]{media.insert({newMedium->id(), std::move(newMedium)})};

    if (success)
        emit mediaChanged();

    return success;
}

bool Library::replaceMedium(std::unique_ptr<const Medium> newMedium) {
    if (newMedium == nullptr || !media.contains(newMedium->id())) {
        return false;
    }
    media.at(newMedium->id()) = std::move(newMedium);
    emit mediaChanged();
    return true;
}

bool Library::removeMedium(const QUuid& id) {
    const bool didRemove{media.erase(id) > 0};
    if (didRemove)
        emit mediaChanged();
    return didRemove;
}

void Library::clear() {
    const auto previousCount{mediaCount()};
    media.clear();
    if (previousCount > 0)
        emit mediaChanged();
}

}