#include "Library.h"

#include <algorithm>
#include <ranges>

namespace Core::Model {

void Library::swap(Library &other) noexcept {
    std::swap(media, other.media);
    if (mediaCount() != 0 || other.mediaCount() != 0) {
        emit sigsEmitter->mediaChanged();
        emit other.sigsEmitter->mediaChanged();
    }
}

Library::Library(const Library &other) {
    for (const auto &medium : other.media | std::views::values) {
        media.try_emplace(medium->id(), medium->clone());
    }
}

auto Library::operator=(const Library &other) -> Library & {
    Library copy{other};
    swap(copy);
    return *this;
}

auto Library::emitter() const -> std::shared_ptr<const LibrarySignals> {
    return sigsEmitter;
}

auto Library::getMedia() const -> std::vector<const Medium *> {
    auto view{media | std::views::values |
              std::views::transform([](const auto &ptr) { return ptr.get(); })};
    return {view.begin(), view.end()};
}

auto Library::getTopicsUnion() const -> std::set<QString> {
    std::set<QString> topics;
    std::ranges::for_each(media, [&](const auto &pair) {
        const auto &[_, medium]{pair};
        topics.merge(medium->userData().topics().get().value_or(std::set<QString>{}));
    });
    return topics;
}

auto Library::getMedium(const QUuid &id) const -> std::optional<const Medium *> {
    if (!media.contains(id)) {
        return std::nullopt;
    }
    return std::make_optional(media.at(id).get());
}

auto Library::mediaCount() const -> size_t {
    return media.size();
}

void Library::setMedia(std::vector<std::unique_ptr<const Medium>> newMedia) {
    const auto previousMediaCount{mediaCount()};

    media.clear();
    for (auto &medium : newMedia) {
        if (medium == nullptr) {
            continue;
        }
        media.try_emplace(medium->id(), std::move(medium));
    }

    if (previousMediaCount != 0) {
        emit sigsEmitter->mediaChanged();
    }
}

void Library::merge(Library other) {
    const auto countBeforeMerge{mediaCount()};
    media.merge(std::move(other.media));

    if (const auto countAfterMerge{mediaCount()}; countBeforeMerge != countAfterMerge) {
        emit sigsEmitter->mediaChanged();
    }
}

auto Library::addMedium(std::unique_ptr<const Medium> newMedium) -> bool {
    if (newMedium == nullptr) {
        return false;
    }

    const auto [_, success]{media.try_emplace(newMedium->id(), std::move(newMedium))};
    if (success) {
        emit sigsEmitter->mediaChanged();
    }
    return success;
}

auto Library::replaceMedium(std::unique_ptr<const Medium> newMedium) -> bool {
    if (newMedium == nullptr || !media.contains(newMedium->id())) {
        return false;
    }
    media.at(newMedium->id()) = std::move(newMedium);
    emit sigsEmitter->mediaChanged();
    return true;
}

auto Library::removeMedium(const QUuid &id) -> bool {
    const bool didRemove{media.erase(id) > 0};
    if (didRemove) {
        emit sigsEmitter->mediaChanged();
    }
    return didRemove;
}

void Library::clear() {
    const auto previousCount{mediaCount()};
    media.clear();
    if (previousCount > 0) {
        emit sigsEmitter->mediaChanged();
    }
}

}