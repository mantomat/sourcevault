#include "Library.h"

#include <ranges>

namespace Core::Model {

std::shared_ptr<Library> Library::library = nullptr;

std::shared_ptr<Library> Library::getLibrary() {
    if (library == nullptr) {
        // ignore linting issues. Library can't be created with std::make_shared as it doesn't
        // provide public constructors. Creating a shared pointer this way is perfectly fine.
        library = std::shared_ptr<Library>{new Library{}}; // NOSONAR
    }
    return library;
}

std::vector<const Medium*> Library::getMediaView() const {
    auto range = media | std::views::transform([](const auto& ptr) { return ptr.get(); });
    return {range.begin(), range.end()};
}

bool Library::setMedia(std::vector<std::unique_ptr<const Medium>>&& newMedia) {
    if (std::ranges::any_of(newMedia, [](const auto& mediaPtr) { return mediaPtr == nullptr; })) {
        return false;
    }
    media.clear();
    addMedia(std::move(newMedia));
    return true;
}

bool Library::addMedia(std::vector<std::unique_ptr<const Medium>>&& newMedia) {
    if (std::ranges::any_of(newMedia, [](const auto& mediaPtr) { return mediaPtr == nullptr; })) {
        return false;
    }

    for (auto& medium : newMedia) {
        media.push_back(std::move(medium));
    }
    emit mediaChanged(getMediaView());
    return true;
}

bool Library::addMedium(std::unique_ptr<const Medium> newMedium) {
    if (newMedium == nullptr) {
        return false;
    }

    media.push_back(std::move(newMedium));
    emit mediaChanged(getMediaView());
    return true;
}

bool Library::replaceMedium(const Medium* oldMedium, std::unique_ptr<const Medium> newMedium) {
    if (const auto oldMediumIterator{
            std::ranges::find_if(media,
                                 [oldMedium](const std::unique_ptr<const Medium>& medium) {
                                     return medium.get() == oldMedium;
                                 })};
        oldMediumIterator != media.end() && newMedium != nullptr) {
        *oldMediumIterator = std::move(newMedium);
        emit mediaChanged(getMediaView());
        return true;
    }
    return false;
}

bool Library::removeMedium(const Medium* mediumToRemove) {
    if (const auto mediumToRemoveIterator{
            std::ranges::find_if(media,
                                 [mediumToRemove](const std::unique_ptr<const Medium>& medium) {
                                     return medium.get() == mediumToRemove;
                                 })};
        mediumToRemoveIterator != media.end()) {
        media.erase(mediumToRemoveIterator);
        emit mediaChanged(getMediaView());
        return true;
    }
    return false;
}

void Library::clearMedia() {
    if (!media.empty()) {
        media.clear();
        emit mediaChanged(getMediaView());
    }
}

std::set<QString> Library::getAllTopics() const {
    std::set<QString> topics;
    std::ranges::for_each(media, [&](const auto& medium) {
        topics.merge(medium->userData().topics().get().value_or(std::set<QString>{}));
    });
    return topics;
}

}