#ifndef LIBRARY_H
#define LIBRARY_H

#include "LibrarySignals.h"
#include "Medium.h"

namespace Core::Model {

/**
 * Manages the vector of media that are currently loaded in memory representing a library.
 */
class Library final {

    std::map<QUuid, std::unique_ptr<const Medium>> media;
    std::shared_ptr<LibrarySignals> sigsEmitter;

    void emitMediaChanged() const;

  public:
    /**
     * @brief Constructs a new library.
     * This class uses dependency injection to decouple the non-copyable and non-movable signal
     * emitter from the library, in order to allow for copy and moves.
     *
     * You can omit the parameter if you don't need to receive updates from the library.
     */
    explicit Library(const std::shared_ptr<LibrarySignals>& signalsEmitter = nullptr);

    Library(Library&&) = default;
    Library& operator=(Library&&) = default;

    Library(const Library& other);
    Library& operator=(const Library& other);

    void swap(Library& other) noexcept;

    const LibrarySignals* signalsEmitter() const;

    /**
     * @brief Returns a read-only view of all media in the library.
     *
     * The returned vector contains raw pointers to the immutable `Medium` objects owned by the
     * library. These pointers are guaranteed to remain valid until the next change to the
     * library's media, such as an insertion, removal, or replacement of a medium. To be
     * notified of such changes and ensure your view remains consistent, connect to the
     * `mediaChanged` signal of the Library class.
     *
     * @return A vector of raw pointers to immutable media objects.
     */
    std::vector<const Medium*> getAllMedia() const;

    /**
     * @brief Returns a set containing every distinct topic in the collection of media.
     */
    std::set<QString> getAllTopics() const;

    /**
     * @brief Returns a non-owning pointer to a medium by its ID. If no such medium exists,
     * std::nullopt is returned.
     *
     * The pointer is guaranteed to remain valid until the next change to the library's media, such
     * as an insertion, removal, or replacement of a medium. To be notified of such changes and
     * ensure your view remains consistent, connect to the `mediaChanged` signal of the Library
     * class.
     *
     * @return A const pointer to the medium, or nullptr if not found.
     */
    std::optional<const Medium*> getMedium(const QUuid& id) const;

    /**
     * @brief Returns the total number of media in the library.
     */
    size_t mediaCount() const;

    /**
     * @brief Sets the entire media collection, erasing anything that was present before.
     *
     * Nullptr's and duplicates in `newMedia` are ignored.
     */
    void setMedia(std::vector<std::unique_ptr<const Medium>> newMedia);

    /**
     * @brief Merges this library with another one, consuming it.
     */
    void merge(std::unique_ptr<Library> other);

    /**
     * @brief Adds a new medium to the library, taking ownership.
     * @return True if the medium was added, false if an item with that ID already exists or the
     * argument was nullptr.
     */
    bool addMedium(std::unique_ptr<const Medium> newMedium);

    /**
     * @brief Replaces an existing medium with a new one, identified by ID.
     * The new medium must have the same ID as the one it's replacing.
     * @return True if the replacement was successful, false if no medium with that ID was found or
     * the argument was nullptr.
     */
    bool replaceMedium(std::unique_ptr<const Medium> newMedium);

    /**
     * @brief Removes a medium from the library by its unique ID.
     * @return True if a medium was found and removed, false otherwise.
     */
    bool removeMedium(const QUuid& id);

    /**
     * @brief Clears the entire collection
     */
    void clear();

  signals:
    void mediaChanged();
};

}

#endif
