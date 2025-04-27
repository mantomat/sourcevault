#ifndef LIBRARY_H
#define LIBRARY_H

#include "Medium.h"

#include <span>

namespace Core::Model {

/**
 * Manages the vector of media that are currently loaded in memory representing the library.
 * Implemented as a singleton.
 */
class Library final : public QObject {
    Q_OBJECT

    std::vector<std::unique_ptr<const Medium>> media;
    static std::shared_ptr<Library> library;

    Library() = default;

  public:
    Library(Library& other) = delete;
    Library(const Library& other) = delete;
    Library(const Library&& other) = delete;
    void operator=(const Library& other) = delete;
    void operator=(const Library&& other) = delete;

    static std::shared_ptr<Library> getLibrary();

    /**
     * Returns a read-only view of the media currently stored in the library.
     *
     * The returned vector contains raw pointers to the immutable `Medium` objects owned by the
     * library. These pointers are guaranteed to remain valid until the next change to the library's
     * media, such as an insertion, removal, or replacement of a medium.
     * To be notified of such changes and ensure your view remains consistent,
     * connect to the `mediaChanged` signal of the Library class.
     *
     * @return A vector of raw pointers to immutable media objects.
     */
    std::vector<const Medium*> getMediaView() const;

    bool setMedia(std::vector<std::unique_ptr<const Medium>>&& newMedia);

    bool addMedia(std::vector<std::unique_ptr<const Medium>>&& newMedia);
    bool addMedium(std::unique_ptr<const Medium> newMedium);

    bool replaceMedium(const Medium* oldMedium, std::unique_ptr<const Medium> newMedium);

    bool removeMedium(const Medium* mediumToRemove);

    void clearMedia();

    std::set<QString> getAllTopics() const;

  signals:
    void mediaChanged(std::vector<const Core::Model::Medium*> newMedia);
};
}

#endif
