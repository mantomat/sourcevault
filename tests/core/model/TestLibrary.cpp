#include "TestLibrary.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/Medium.h"
#include "model/Video.h"

#include <QSignalSpy>
#include <QTest>
#include <algorithm>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Medium;
using Core::Model::Video;

void TestLibrary::populateLib(Library& libToPopulate) const {
    std::vector<std::unique_ptr<const Medium>> mediaToSet;
    mediaToSet.push_back(std::make_unique<Book>(book));
    mediaToSet.push_back(std::make_unique<Video>(video));
    mediaToSet.push_back(std::make_unique<Article>(article));
    libToPopulate.setMedia(std::move(mediaToSet));
}

void TestLibrary::testGetAllMedia() const {
    Library lib;
    QCOMPARE(lib.getAllMedia().size(), 0);

    populateLib(lib);
    std::set expected{expectedIds};

    const auto mediaView{lib.getAllMedia()};
    QCOMPARE(mediaView.size(), expected.size());
    for (const Medium* mediumPtr : mediaView) {
        QVERIFY(mediumPtr != nullptr);
        QVERIFY(expected.contains(mediumPtr->id()));
        expected.erase(mediumPtr->id());
    }
}

void TestLibrary::testGetAllTopics() const {
    Library lib;
    QCOMPARE(lib.getAllTopics().size(), 0);

    populateLib(lib);
    std::set expected{expectedTopics};

    const auto actualTopics{lib.getAllTopics()};
    QCOMPARE(actualTopics.size(), expected.size());
    for (const auto& topic : actualTopics) {
        QVERIFY(expected.contains(topic));
        expected.erase(topic);
    }
}

void TestLibrary::testGetMedium_data() const {
    QTest::addColumn<QUuid>("idToGet");
    QTest::addColumn<bool>("shouldBeFound");

    Library lib;
    populateLib(lib);

    QTest::addRow("A null id must never be found") << QUuid{} << false;
    QTest::addRow("An absent id must never be found") << QUuid{QUuid::createUuid()} << false;
    QTest::addRow("A present id must return the corresponding medium") << book.id() << true;
}

void TestLibrary::testGetMedium() const {
    QFETCH(QUuid, idToGet);
    QFETCH(bool, shouldBeFound);

    // Creating this every time could slow down the tests, but it's the easiest thing to do.
    Library lib;
    populateLib(lib);

    const auto optionalMedium{lib.getMedium(idToGet)};
    QCOMPARE(optionalMedium.has_value(), shouldBeFound);
}

void TestLibrary::testMediaCount() const {
    Library lib;
    QCOMPARE(lib.mediaCount(), 0);

    populateLib(lib);

    QCOMPARE(lib.mediaCount(), expectedCount);
}

void TestLibrary::testSetMedia_data() {
    using cMediaPtr = std::unique_ptr<const Medium>;
    using MediaVector = std::vector<cMediaPtr>;
    using MediaGenerator = std::function<MediaVector()>;

    // A generator is used because vectors of unique_ptrs are not copyable, thus they cannot be
    // passed to data driven test functions in Qt.
    QTest::addColumn<MediaGenerator>("mediaGenerator");
    QTest::addColumn<size_t>("expectedCountAfterSet");

    QTest::addRow("Setting an empty vector")
        << MediaGenerator{[] { return MediaVector{}; }} << size_t{0};

    QTest::addRow("Nullptr's must be ignored") << MediaGenerator{[] {
        MediaVector vec;
        vec.push_back(std::make_unique<Book>(Book::create("Valid Book").value()));
        vec.push_back(nullptr);
        vec.push_back(std::make_unique<Article>(Article::create("Valid Article").value()));
        vec.push_back(nullptr);
        return vec;
    }} << size_t{2};

    QTest::addRow("Duplicates are ignored") << MediaGenerator{[] {
        MediaVector vec;
        const auto book = Book::create("Same ID Book").value();
        vec.push_back(std::make_unique<Book>(book));
        vec.push_back(std::make_unique<Book>(book));
        return vec;
    }} << size_t{1};
}
void TestLibrary::testSetMedia() {
    QFETCH(std::function<std::vector<std::unique_ptr<const Medium>>()>, mediaGenerator);
    QFETCH(size_t, expectedCountAfterSet);

    std::vector<std::unique_ptr<const Medium>> mediaToSet = mediaGenerator();
    Library lib;
    const QSignalSpy spy{&lib, &Library::mediaChanged};

    lib.setMedia(std::move(mediaToSet));

    QCOMPARE(lib.mediaCount(), expectedCountAfterSet);
    QCOMPARE(spy.count(), 1);
}

void TestLibrary::testMerge_data() const {
    using LibraryGenerator = std::function<std::unique_ptr<Library>()>;

    QTest::addColumn<LibraryGenerator>("destinationGenerator");
    QTest::addColumn<LibraryGenerator>("sourceGenerator");
    QTest::addColumn<std::set<QUuid>>("expectedFinalIds");
    QTest::addColumn<bool>("expectedSignalEmission");

    QTest::addRow("Disjunct merge acts as a disjunct union") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Video>(video));
        return lib;
    }} << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Article>(article));
        return lib;
    }} << std::set{book.id(), video.id(), article.id()} << true;

    QTest::addRow("Merging with an empty library does nothing") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Video>(video));
        return lib;
    }} << LibraryGenerator{[] { return std::make_unique<Library>(); }}
                                                                << std::set{book.id(), video.id()}
                                                                << false;

    QTest::addRow("If duplicates are found, they are ignored") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Video>(video));
        return lib;
    }} << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << std::set{book.id(), video.id()} << false;

    QTest::addRow("If duplicates are found, they are ignored") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Video>(video));
        return lib;
    }} << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Article>(article));
        return lib;
    }} << std::set{book.id(), video.id(), article.id()} << true;

    QTest::addRow("If source is nullptr, nothing happens") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        lib->addMedium(std::make_unique<Video>(video));
        return lib;
    }} << LibraryGenerator{[=] { return std::unique_ptr<Library>{nullptr}; }}
                                                           << std::set{book.id(), video.id()}
                                                           << false;
}
void TestLibrary::testMerge() {
    QFETCH(std::function<std::unique_ptr<Library>()>, destinationGenerator);
    QFETCH(std::function<std::unique_ptr<Library>()>, sourceGenerator);
    QFETCH(std::set<QUuid>, expectedFinalIds);
    QFETCH(bool, expectedSignalEmission);

    const auto destination{destinationGenerator()};
    auto source{sourceGenerator()};
    const QSignalSpy spy{destination.get(), &Library::mediaChanged};

    destination->merge(std::move(source));

    auto media{destination->getAllMedia()};
    auto idView{media | std::views::transform([](auto m) { return m->id(); })};
    const std::set<QUuid> actualIds{idView.begin(), idView.end()};

    QCOMPARE(actualIds, expectedFinalIds);
    QCOMPARE(spy.count(), expectedSignalEmission ? 1 : 0);
}

void TestLibrary::testAddMedium_data() const {
    using LibraryGenerator = std::function<std::unique_ptr<Library>()>;
    using MediumGenerator = std::function<std::unique_ptr<const Medium>()>;

    QTest::addColumn<LibraryGenerator>("libraryGenerator");
    QTest::addColumn<MediumGenerator>("mediumToAddGenerator");
    QTest::addColumn<bool>("shouldBeAdded");

    QTest::addRow("Adding a new (non-duplicate) medium returns true")
        << LibraryGenerator{[] { return std::make_unique<Library>(); }}
        << MediumGenerator{[this] { return std::make_unique<Book>(book); }} << true;

    QTest::addRow("Adding a duplicate medium does nothing and returns false")
        << LibraryGenerator{[this] {
               auto lib{std::make_unique<Library>()};
               lib->addMedium(std::make_unique<Book>(book));
               return lib;
           }}
        << MediumGenerator{[this] { return std::make_unique<Book>(book); }} << false;

    QTest::addRow("Adding a nullptr does nothing and returns false") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << MediumGenerator{[] { return std::unique_ptr<Medium>{nullptr}; }}
                                                                     << false;
}
void TestLibrary::testAddMedium() {
    QFETCH(std::function<std::unique_ptr<Library>()>, libraryGenerator);
    QFETCH(std::function<std::unique_ptr<const Medium>()>, mediumToAddGenerator);
    QFETCH(bool, shouldBeAdded);

    const auto lib{libraryGenerator()};
    const QSignalSpy spy{lib.get(), &Library::mediaChanged};

    const bool wasAdded{lib->addMedium(mediumToAddGenerator())};

    QCOMPARE(wasAdded, shouldBeAdded);
    QCOMPARE(spy.count(), shouldBeAdded ? 1 : 0);
}

void TestLibrary::testReplaceMedium_data() const {
    using LibraryGenerator = std::function<std::unique_ptr<Library>()>;
    using MediumGenerator = std::function<std::unique_ptr<const Medium>()>;

    QTest::addColumn<LibraryGenerator>("libraryGenerator");
    QTest::addColumn<MediumGenerator>("newMediumGenerator");
    QTest::addColumn<bool>("shouldBeReplaced");

    QTest::addRow("Replacing existing medium returns true") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << MediumGenerator{[this] {
        auto replacedBook{std::make_unique<Book>(book)};
        replacedBook->setTitle("replaced");
        return replacedBook;
    }} << true;

    QTest::addRow("Replacing absent medium does nothing and returns false")
        << LibraryGenerator{[this] {
               auto lib{std::make_unique<Library>()};
               lib->addMedium(std::make_unique<Book>(book));
               return lib;
           }}
        << MediumGenerator{[this] {
               auto replacedBook{std::make_unique<Video>(video)};
               replacedBook->setTitle("replaced");
               return replacedBook;
           }}
        << false;

    QTest::addRow("Passing nullptr does nothing and returns false") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << MediumGenerator{[] { return std::unique_ptr<Book>{nullptr}; }}
                                                                    << false;
}
void TestLibrary::testReplaceMedium() {
    QFETCH(std::function<std::unique_ptr<Library>()>, libraryGenerator);
    QFETCH(std::function<std::unique_ptr<const Medium>()>, newMediumGenerator);
    QFETCH(bool, shouldBeReplaced);

    const auto lib{libraryGenerator()};
    const QSignalSpy spy{lib.get(), &Library::mediaChanged};

    const bool wasReplaced{lib->replaceMedium(newMediumGenerator())};

    QCOMPARE(wasReplaced, shouldBeReplaced);
    QCOMPARE(spy.count(), shouldBeReplaced ? 1 : 0);

    // A medium named "replaced" must be present iff shouldBeReplaced is true in this test.
    const auto& media = lib->getAllMedia();
    const bool isThereReplaced{
        std::ranges::any_of(media | std::views::transform([](auto m) { return m->title(); }),
                            [](const auto& title) { return title == "replaced"; })};
    QVERIFY((shouldBeReplaced && isThereReplaced) || (!shouldBeReplaced && !isThereReplaced));
}

void TestLibrary::testRemoveMedium_data() const {
    using LibraryGenerator = std::function<std::unique_ptr<Library>()>;

    QTest::addColumn<LibraryGenerator>("libraryGenerator");
    QTest::addColumn<QUuid>("mediumToRemoveId");
    QTest::addColumn<bool>("shouldBeRemoved");

    QTest::addRow("Removing an existing medium") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << book.id() << true;

    QTest::addRow("Trying to remove an absent medium does nothing") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << video.id() << false;
}
void TestLibrary::testRemoveMedium() {
    QFETCH(std::function<std::unique_ptr<Library>()>, libraryGenerator);
    QFETCH(QUuid, mediumToRemoveId);
    QFETCH(bool, shouldBeRemoved);

    const auto lib{libraryGenerator()};
    const QSignalSpy spy{lib.get(), &Library::mediaChanged};
    const auto& wasMediumPresent{std::ranges::any_of(
        lib->getAllMedia(), [&](auto mPtr) { return mPtr->id() == mediumToRemoveId; })};

    const bool wasRemoved{lib->removeMedium(mediumToRemoveId)};

    QCOMPARE(wasRemoved, shouldBeRemoved);
    QCOMPARE(spy.count(), shouldBeRemoved ? 1 : 0);

    // If the medium was present and shouldBeRemoved is true, then it must be now absent
    const bool isMediumPresent{std::ranges::any_of(
        lib->getAllMedia(), [&](auto mPtr) { return mPtr->id() == mediumToRemoveId; })};
    QVERIFY(!(wasMediumPresent && shouldBeRemoved) || !isMediumPresent);
}

void TestLibrary::testClear_data() const {
    using LibraryGenerator = std::function<std::unique_ptr<Library>()>;

    QTest::addColumn<LibraryGenerator>("libraryGenerator");
    QTest::addColumn<bool>("shouldEmit");

    QTest::addRow("Clearing an empty library doesn't emit")
        << LibraryGenerator{[] { return std::make_unique<Library>(); }} << false;

    QTest::addRow("Clearing a non-empty library emits a signal") << LibraryGenerator{[this] {
        auto lib{std::make_unique<Library>()};
        lib->addMedium(std::make_unique<Book>(book));
        return lib;
    }} << true;
}

void TestLibrary::testClear() {
    QFETCH(std::function<std::unique_ptr<Library>()>, libraryGenerator);
    QFETCH(bool, shouldEmit);

    const auto lib{libraryGenerator()};
    const QSignalSpy spy{lib.get(), &Library::mediaChanged};

    lib->clear();
    QCOMPARE(lib->mediaCount(), size_t{0});
    QCOMPARE(spy.count(), shouldEmit ? 1 : 0);
}
