#ifndef LIBRARY_H
#define LIBRARY_H

#include "Medium.h"

#include <QUuid>

/**
 * Implementation of the std::hash function for QUuid, needed to use it as key in unordered_map
 */
template <> struct std::hash<QUuid> {
    auto operator()(const QUuid &uuid) const -> std::size_t {
        return qHash(uuid);
    }
};

namespace Core::Model {

/**
 * This struct is designed to decouple the signal emitter (QObject) from Library.
 *
 * A QObject's purpose is to be a unique "identity" within Qt's object tree. It has a parent,
 * children, and a unique identity that cannot be copied or moved.
 *
 * On the contrary, Library is a "value" class. It represents the data that lies beneath it, it
 * doesn't have an identity. But we needed to design some kind of observer pattern for it, and
 * overlooking the powerful Qt's signaling system would have been a bad choice.
 */
struct LibrarySignals final : public QObject {
    Q_OBJECT
  signals:
    void mediaChanged() const;
};

/**
 * @brief Manages a collection of media representing a library.
 */
class Library final {

    std::unordered_map<QUuid, std::unique_ptr<const Medium>> media;
    std::unique_ptr<const LibrarySignals> sigsEmitter{std::make_unique<LibrarySignals>()};

    /**
     * @brief Swaps the current library with `other`.
     * This operation will emit a `mediaChanged` signal on both libraries if they were not empty.
     */
    void swap(Library &other) noexcept;

  public:
    ~Library() = default;
    explicit Library() = default;
    Library(Library &&) = default;
    auto operator=(Library &&) -> Library & = default;

    /**
     * @brief Copies a library, performing a deep copy of media.
     */
    Library(const Library &other);

    /**
     * @brief Copy-assigns a library, performing a deep copy of media.
     *
     * If at least one between this library and `other` wasn't empty, emits a `mediaChanged` signal.
     */
    auto operator=(const Library &other) -> Library &;

    auto operator==(const Library &other) const -> bool;

    /**
     * @brief Returns a non-owning pointer to the library emitter.
     *
     * This pointer will be valid as long as the library lives.
     */
    [[nodiscard]] auto emitter() const -> const LibrarySignals *;

    /**
     * @return An vector of raw pointers to immutable media objects, in a random order.
     *
     * The returned vector contains raw pointers to the immutable `Medium` objects owned by the
     * library. These pointers are guaranteed to remain valid until the next change to the
     * library's media, such as an insertion, removal, or replacement of a medium. To be
     * notified of such changes and ensure your view remains consistent, connect to the
     * `mediaChanged` signal of the Library class.
     */
    [[nodiscard]] auto getMedia() const -> std::vector<const Medium *>;

    /**
     * @brief Returns a set containing every distinct topic in the collection of media.
     */
    [[nodiscard]] auto getTopicsUnion() const -> std::set<QString>;

    /**
     * @brief Returns a non-owning pointer to a medium by its ID.
     *
     * The pointer is guaranteed to remain valid until the next change to the library's media, such
     * as an insertion, removal, or replacement of a medium. To be notified of such changes and
     * ensure your view remains consistent, connect to the `mediaChanged` signal of the Library
     * class.
     *
     * @return A const pointer to the medium, or nullopt if not found.
     */
    [[nodiscard]] auto getMedium(const QUuid &id) const -> std::optional<const Medium *>;

    /**
     * @return The total number of media in the library.
     */
    [[nodiscard]] auto mediaCount() const -> size_t;

    /**
     * @brief Sets the entire media collection, erasing anything that was present before.
     *
     * Nullptr's and duplicates in `newMedia` are ignored.
     */
    void setMedia(std::vector<std::unique_ptr<const Medium>> newMedia);

    /**
     * @brief Merges this library with another one, consuming it.
     */
    void merge(Library other);

    /**
     * @brief Adds a new medium to the library, taking ownership.
     * @return True if the medium was added, false if an item with that ID already exists or the
     * argument was nullptr.
     */
    auto addMedium(std::unique_ptr<const Medium> newMedium) -> bool;

    /**
     * @brief Replaces an existing medium with a new one, identified by ID.
     * The new medium must have the same ID as the one it's replacing.
     * @return True if the replacement was successful, false if no medium with that ID was found or
     * the argument was nullptr.
     */
    auto replaceMedium(std::unique_ptr<const Medium> newMedium) -> bool;

    /**
     * @brief Removes a medium from the library by its unique ID.
     * @return True if a medium was found and removed, false otherwise.
     */
    auto removeMedium(const QUuid &id) -> bool;

    /**
     * @brief Clears the entire collection
     */
    void clear();
};

}

#endif
