#include "TestLibrary.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/Medium.h"
#include "model/Video.h"

#include <QSignalSpy>
#include <QTest>
#include <algorithm>
#include <qtestcase.h>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::LibrarySignals;
using Core::Model::Medium;
using Core::Model::Video;

void TestLibrary::testCopyConstructor_data() const {
    QTest::addColumn<Library>("libraryToCopy");

    QTest::addRow("Copying an empty library creates an empty library with a fresh new emitter.")
        << Library{};
    QTest::addRow(
        "Copying a non-empty library creates a deep copy of the media and a fresh new emitter.")
        << defaultLibrary;
}
void TestLibrary::testCopyConstructor() {
    QFETCH(Library, libraryToCopy);

    // Since it's exactly what we want:
    // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
    Library newLib{libraryToCopy};

    QVERIFY(libraryToCopy.emitter() != newLib.emitter());

    std::vector originals{libraryToCopy.getMedia()};
    std::vector deepCopies{newLib.getMedia()};

    QCOMPARE(originals.size(), deepCopies.size());

    auto sortById = [](const Medium *a, const Medium *b) { return a->id() < b->id(); };
    std::ranges::sort(originals, sortById);
    std::ranges::sort(deepCopies, sortById);
    for (size_t i = 0; i < originals.size(); i++) {
        QVERIFY(originals[i] != deepCopies[i]);
        QCOMPARE(originals[i]->id(), deepCopies[i]->id());
    }
}

void TestLibrary::testCopyAssignment_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<Library>("libraryToCopyAssign");
    QTest::addColumn<bool>("expectedSignalEmission");

    QTest::addRow(
        "Copy-assigning an empty library to an empty library does nothing and doesn't emit.")
        << Library{} << Library{} << false;
    QTest::addRow("Copy-assigning a non empty library to an empty library performs a deep copy and "
                  "emits a signal.")
        << Library{} << defaultLibrary << true;
    QTest::addRow("Copy-assigning an empty library to a non-empty library emits a signal.")
        << Library{} << defaultLibrary << true;
}
void TestLibrary::testCopyAssignment() {
    QFETCH(Library, library);
    QFETCH(Library, libraryToCopyAssign);
    QFETCH(bool, expectedSignalEmission);

    QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};

    library = libraryToCopyAssign;

    std::vector libraryToCopyMedia{libraryToCopyAssign.getMedia()};
    std::vector libraryMedia{library.getMedia()};

    QCOMPARE(libraryToCopyMedia.size(), libraryMedia.size());

    auto sortById = [](const Medium *a, const Medium *b) { return a->id() < b->id(); };
    std::ranges::sort(libraryToCopyMedia, sortById);
    std::ranges::sort(libraryMedia, sortById);
    for (size_t i = 0; i < libraryToCopyMedia.size(); i++) {
        QVERIFY(libraryToCopyMedia[i] != libraryMedia[i]);
        QCOMPARE(libraryToCopyMedia[i]->id(), libraryMedia[i]->id());
    }
    QCOMPARE(spy.count(), expectedSignalEmission ? 1 : 0);
}

void TestLibrary::testGetAllMedia_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<std::set<QUuid>>("expectedMediaIds");

    QTest::addRow("An empty library has no media in it") << Library{} << std::set<QUuid>{};
    QTest::addRow("A library with some media")
        << defaultLibrary
        << std::set<QUuid>{defaultBook.id(), defaultArticle.id(), defaultVideo.id()};
}
void TestLibrary::testGetAllMedia() {
    QFETCH(Library, library);
    QFETCH(std::set<QUuid>, expectedMediaIds);

    const auto &media = library.getMedia();
    auto idView = media | std::views::transform([](const auto m) { return m->id(); });
    std::set<QUuid> actualIds{idView.begin(), idView.end()};

    QCOMPARE(actualIds, expectedMediaIds);
}

void TestLibrary::testGetAllTopics_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<std::set<QString>>("expectedTopics");

    QTest::addRow("An empty library has no topics in it") << Library{} << std::set<QString>{};

    std::set<QString> expectedTopics{};
    expectedTopics.merge(defaultBook.userData().topics().get().value());
    expectedTopics.merge(defaultArticle.userData().topics().get().value());
    expectedTopics.merge(defaultVideo.userData().topics().get().value());
    QTest::addRow("A library with some topics") << defaultLibrary << expectedTopics;
}

void TestLibrary::testGetAllTopics() {
    QFETCH(Library, library);
    QFETCH(std::set<QString>, expectedTopics);

    QCOMPARE(library.getTopicsUnion(), expectedTopics);
}

void TestLibrary::testGetMedium_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<QUuid>("idToGet");
    QTest::addColumn<bool>("shouldBeFound");

    QTest::addRow("A null id must never be found") << defaultLibrary << QUuid{} << false;
    QTest::addRow("An absent id must never be found")
        << defaultLibrary << QUuid{QUuid::createUuid()} << false;
    QTest::addRow("A present id must return the corresponding medium")
        << defaultLibrary << defaultBook.id() << true;
}

void TestLibrary::testGetMedium() {
    QFETCH(Library, library);
    QFETCH(QUuid, idToGet);
    QFETCH(bool, shouldBeFound);

    const auto optionalMedium{library.getMedium(idToGet)};
    QCOMPARE(optionalMedium.has_value(), shouldBeFound);
}

void TestLibrary::testMediaCount_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<size_t>("expectedCount");

    QTest::addRow("A (new) empty library has no media") << Library{} << size_t{0};
    QTest::addRow("A library with some media") << defaultLibrary << size_t{3};
}

void TestLibrary::testMediaCount() {
    QFETCH(Library, library);
    QFETCH(size_t, expectedCount);

    QCOMPARE(library.mediaCount(), expectedCount);
}

void TestLibrary::testSetMedia_data() const {
    using MediaVector = std::vector<std::unique_ptr<const Medium>>;
    using MediaGenerator = std::function<MediaVector()>;

    QTest::addColumn<Library>("library");
    // A generator is used because vectors of unique_ptrs are not copyable, thus they cannot be
    // passed to data driven test functions in Qt Test.
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<size_t>("expectedCountAfterSet");
    QTest::addColumn<bool>("expectedSignalEmission");

    QTest::addRow("Setting an empty vector on an empty library doesn't emit any signals")
        << Library{} << MediaGenerator{[] { return MediaVector{}; }} << size_t{0} << false;
    QTest::addRow("Setting an empty vector on a non-empty library emits a mediaChanged signal")
        << defaultLibrary << MediaGenerator{[this] {
               MediaVector vec;
               vec.push_back(std::make_unique<Book>(defaultBook));
               return vec;
           }}
        << size_t{1} << true;
    QTest::addRow("Nullptr's are ignored") << defaultLibrary << MediaGenerator{[this] {
        MediaVector vec;
        vec.push_back(std::make_unique<Book>(defaultBook));
        vec.push_back(nullptr);
        vec.push_back(std::make_unique<Article>(defaultArticle));
        vec.push_back(nullptr);
        return vec;
    }} << size_t{2} << true;
    QTest::addRow("Duplicates are ignored") << Library{} << MediaGenerator{[this] {
        MediaVector vec;
        vec.push_back(std::make_unique<Book>(defaultBook));
        vec.push_back(std::make_unique<Book>(defaultBook));
        return vec;
    }} << size_t{1} << false;
}
void TestLibrary::testSetMedia() {
    QFETCH(Library, library);
    QFETCH(std::function<std::vector<std::unique_ptr<const Medium>>()>, mediaGenerator);
    QFETCH(size_t, expectedCountAfterSet);
    QFETCH(bool, expectedSignalEmission);

    std::vector<std::unique_ptr<const Medium>> mediaToSet{mediaGenerator()};
    const QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};

    library.setMedia(std::move(mediaToSet));

    QCOMPARE(library.mediaCount(), expectedCountAfterSet);
    QCOMPARE(spy.count(), expectedSignalEmission ? 1 : 0);
}

void TestLibrary::testMerge_data() const {
    QTest::addColumn<Library>("destination");
    QTest::addColumn<Library>("source");
    QTest::addColumn<std::set<QUuid>>("expectedFinalIds");
    QTest::addColumn<bool>("expectedSignalEmission");

    {
        Library destination;
        destination.addMedium(std::make_unique<Book>(defaultBook));
        destination.addMedium(std::make_unique<Video>(defaultVideo));
        Library source;
        source.addMedium(std::make_unique<Article>(defaultArticle));
        QTest::addRow("Disjunct merge acts as a disjunct union")
            << destination << source
            << std::set{defaultBook.id(), defaultVideo.id(), defaultArticle.id()} << true;
    }

    {
        Library destination;
        destination.addMedium(std::make_unique<Book>(defaultBook));
        destination.addMedium(std::make_unique<Video>(defaultVideo));
        Library source;
        QTest::addRow("Merging with an empty library does nothing")
            << destination << source << std::set{defaultBook.id(), defaultVideo.id()} << false;
    }

    {
        Library destination;
        Library source;
        source.addMedium(std::make_unique<Book>(defaultBook));
        source.addMedium(std::make_unique<Video>(defaultVideo));
        QTest::addRow("Merging an empty library with a non-empty library acts as an assignment")
            << destination << source << std::set{defaultBook.id(), defaultVideo.id()} << true;
    }

    {
        Library destination;
        destination.addMedium(std::make_unique<Book>(defaultBook));
        destination.addMedium(std::make_unique<Video>(defaultVideo));
        Library source;
        source.addMedium(std::make_unique<Book>(defaultBook));
        source.addMedium(std::make_unique<Article>(defaultArticle));
        QTest::addRow("If duplicates are found, they are ignored")
            << destination << source
            << std::set{defaultBook.id(), defaultVideo.id(), defaultArticle.id()} << true;
    }
}
void TestLibrary::testMerge() {
    QFETCH(Library, destination);
    QFETCH(Library, source);
    QFETCH(std::set<QUuid>, expectedFinalIds);
    QFETCH(bool, expectedSignalEmission);

    const QSignalSpy spy{destination.emitter().get(), &LibrarySignals::mediaChanged};

    destination.merge(std::move(source));

    auto media{destination.getMedia()};
    auto idView{media | std::views::transform([](auto m) { return m->id(); })};
    const std::set<QUuid> actualIds{idView.begin(), idView.end()};

    QCOMPARE(actualIds, expectedFinalIds);
    QCOMPARE(spy.count(), expectedSignalEmission ? 1 : 0);
}

void TestLibrary::testAddMedium_data() const {
    using MediumGenerator = std::function<std::unique_ptr<const Medium>()>;

    QTest::addColumn<Library>("library");
    QTest::addColumn<MediumGenerator>("mediumToAddGenerator");
    QTest::addColumn<bool>("shouldBeAdded");

    QTest::addRow("Adding a new (non-duplicate) medium returns true")
        << Library{} << MediumGenerator{[this] { return std::make_unique<Book>(defaultBook); }}
        << true;
    QTest::addRow("Trying to add a duplicate medium does nothing and returns false")
        << defaultLibrary << MediumGenerator{[this] { return std::make_unique<Book>(defaultBook); }}
        << false;
    QTest::addRow("Adding a nullptr does nothing and returns false")
        << Library{} << MediumGenerator{[] { return std::unique_ptr<Medium>{nullptr}; }} << false;
}
void TestLibrary::testAddMedium() {
    QFETCH(Library, library);
    QFETCH(std::function<std::unique_ptr<const Medium>()>, mediumToAddGenerator);
    QFETCH(bool, shouldBeAdded);

    const QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};

    const bool wasAdded{library.addMedium(mediumToAddGenerator())};

    QCOMPARE(wasAdded, shouldBeAdded);
    QCOMPARE(spy.count(), shouldBeAdded ? 1 : 0);
}

void TestLibrary::testReplaceMedium_data() const {
    using MediumGenerator = std::function<std::unique_ptr<const Medium>()>;

    QTest::addColumn<Library>("library");
    QTest::addColumn<MediumGenerator>("newMediumGenerator");
    QTest::addColumn<bool>("shouldBeReplaced");

    // Convention: no media named "replaced" were present in the library.
    QTest::addRow("Replacing existing medium returns true")
        << defaultLibrary << MediumGenerator{[this] {
               auto replacedBook{std::make_unique<Book>(defaultBook)};
               replacedBook->setTitle("replaced");
               return replacedBook;
           }}
        << true;
    QTest::addRow("Trying to replace an absent medium does nothing and returns false")
        << Library{} << MediumGenerator{[this] {
               auto replacedBook{std::make_unique<Video>(defaultVideo)};
               replacedBook->setTitle("replaced");
               return replacedBook;
           }}
        << false;
    QTest::addRow("Passing nullptr does nothing and returns false")
        << defaultLibrary << MediumGenerator{[] { return std::unique_ptr<Book>{nullptr}; }}
        << false;
}
void TestLibrary::testReplaceMedium() {
    QFETCH(Library, library);
    QFETCH(std::function<std::unique_ptr<const Medium>()>, newMediumGenerator);
    QFETCH(bool, shouldBeReplaced);

    const QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};

    const bool wasReplaced{library.replaceMedium(newMediumGenerator())};

    QCOMPARE(wasReplaced, shouldBeReplaced);
    QCOMPARE(spy.count(), shouldBeReplaced ? 1 : 0);

    // A medium named "replaced" must be present iff shouldBeReplaced is true in this test.
    const auto &media = library.getMedia();
    const bool isThereReplaced{
        std::ranges::any_of(media | std::views::transform([](auto m) { return m->title(); }),
                            [](const auto &title) { return title == "replaced"; })};
    QVERIFY((shouldBeReplaced && isThereReplaced) || (!shouldBeReplaced && !isThereReplaced));
}

void TestLibrary::testRemoveMedium_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<QUuid>("mediumToRemoveId");
    QTest::addColumn<bool>("shouldBeRemoved");

    QTest::addRow("Removing an existing medium") << defaultLibrary << defaultBook.id() << true;

    QTest::addRow("Trying to remove an absent medium does nothing")
        << Library{} << defaultVideo.id() << false;
}
void TestLibrary::testRemoveMedium() {
    QFETCH(Library, library);
    QFETCH(QUuid, mediumToRemoveId);
    QFETCH(bool, shouldBeRemoved);

    const QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};
    const auto &wasMediumPresent{std::ranges::any_of(
        library.getMedia(), [&](auto mPtr) { return mPtr->id() == mediumToRemoveId; })};

    const bool wasRemoved{library.removeMedium(mediumToRemoveId)};

    QCOMPARE(wasRemoved, shouldBeRemoved);
    QCOMPARE(spy.count(), shouldBeRemoved ? 1 : 0);

    // If the medium was present and shouldBeRemoved is true, then it must be now absent
    const bool isMediumPresent{std::ranges::any_of(
        library.getMedia(), [&](auto mPtr) { return mPtr->id() == mediumToRemoveId; })};
    QVERIFY(!(wasMediumPresent && shouldBeRemoved) || !isMediumPresent);
}

void TestLibrary::testClear_data() const {
    QTest::addColumn<Library>("library");
    QTest::addColumn<bool>("shouldEmit");

    QTest::addRow("Clearing an empty library doesn't emit") << Library{} << false;

    QTest::addRow("Clearing a non-empty library emits a signal") << defaultLibrary << true;
}

void TestLibrary::testClear() {
    QFETCH(Library, library);
    QFETCH(bool, shouldEmit);

    const QSignalSpy spy{library.emitter().get(), &LibrarySignals::mediaChanged};

    library.clear();
    QCOMPARE(library.mediaCount(), size_t{0});
    QCOMPARE(spy.count(), shouldEmit ? 1 : 0);
}
