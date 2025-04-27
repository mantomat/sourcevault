#include "TestLibrary.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/Medium.h"
#include "model/Video.h"

#include <QSignalSpy>
#include <QTest>
#include <ranges>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Medium;
using Core::Model::Video;

void TestLibrary::cleanup() {
    // reset the library after every test function
    const std::shared_ptr library{Library::getLibrary()};
    library->clearMedia();

    // Currently, the only two pointing at the library should be the library class and this
    // function.
    QCOMPARE(library.use_count(), 2);
}

void TestLibrary::testGetLibrary() {
    const std::shared_ptr lib1{Library::getLibrary()};
    const std::shared_ptr lib2{Library::getLibrary()};

    // the two should point at the same library
    QCOMPARE(lib1->getMediaView().size(), 0);
    QCOMPARE(lib1->getMediaView(), lib2->getMediaView());

    // when the library is updated somewhere, changes are reflected on all the pointers
    lib1->addMedium(std::make_unique<Book>());
    QCOMPARE(lib1->getMediaView().size(), 1);
    QCOMPARE(lib1->getMediaView(), lib2->getMediaView());

    // when the library is updated somewhere, changes are reflected on all the pointers
    lib2->removeMedium(lib1->getMediaView().at(0));
    QCOMPARE(lib1->getMediaView().size(), 0);
    QCOMPARE(lib1->getMediaView(), lib2->getMediaView());
}

void TestLibrary::testGetMediaView() {
    const std::shared_ptr lib{Library::getLibrary()};

    std::unique_ptr<Medium> firstMedium{std::make_unique<Book>()};
    QString firstTitle{"This is the first medium in the library"};
    firstMedium->title().set(firstTitle);
    lib->addMedium(std::move(firstMedium));

    std::unique_ptr<Medium> secondMedium{std::make_unique<Video>()};
    QString secondTitle{"This is the second medium in the library"};
    secondMedium->title().set(secondTitle);
    lib->addMedium(std::move(secondMedium));

    const std::vector mediaView{lib->getMediaView()};
    QCOMPARE(mediaView.size(), 2);
    QCOMPARE(mediaView.at(0)->title().get(), firstTitle);
    QCOMPARE(mediaView.at(1)->title().get(), secondTitle);
}

void TestLibrary::testSetMedia() {
    const std::shared_ptr library{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{library.get(), &Library::mediaChanged};

    std::vector<std::unique_ptr<const Medium>> media{};
    media.push_back(std::make_unique<Book>());
    media.push_back(std::make_unique<Video>());
    const bool hasBeenSet{library->setMedia(std::move(media))};

    auto view = library->getMediaView();
    QCOMPARE(hasBeenSet, true);
    QCOMPARE(view.size(), 2);
    QVERIFY(dynamic_cast<const Book*>(view.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Video*>(view.at(1)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 1);
    QCOMPARE(mediaChangedSpy.at(0).at(0).value<std::vector<const Medium*>>(), view);

    // Overwrite the vector that has just been set
    std::vector<std::unique_ptr<const Medium>> newMedia{};
    newMedia.push_back(std::make_unique<Video>());
    newMedia.push_back(std::make_unique<Book>());
    newMedia.push_back(std::make_unique<Article>());
    const bool newHasBeenSet{library->setMedia(std::move(newMedia))};

    view = library->getMediaView();
    QCOMPARE(newHasBeenSet, true);
    QCOMPARE(view.size(), 3);
    QVERIFY(dynamic_cast<const Video*>(view.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Book*>(view.at(1)) != nullptr);
    QVERIFY(dynamic_cast<const Article*>(view.at(2)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 2);
    QCOMPARE(mediaChangedSpy.at(1).at(0).value<std::vector<const Medium*>>(), view);
}
void TestLibrary::testSetMediaInvalid() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};

    std::vector<std::unique_ptr<const Medium>> invalidMedia{};
    invalidMedia.push_back(std::make_unique<Book>());
    invalidMedia.push_back(nullptr);
    const bool result{lib->setMedia(std::move(invalidMedia))};
    QCOMPARE(result, false);
    QCOMPARE(lib->getMediaView().size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 0);
}

void TestLibrary::testAddMedia() {
    const std::shared_ptr library{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{library.get(), &Library::mediaChanged};

    std::unique_ptr<Medium> firstMedium{std::make_unique<Book>()};
    QString firstTitle{"This is the first medium in the library"};
    firstMedium->title().set(firstTitle);

    std::unique_ptr<Medium> secondMedium{std::make_unique<Video>()};
    QString secondTitle{"This is the second medium in the library"};
    secondMedium->title().set(secondTitle);

    std::vector<std::unique_ptr<const Medium>> mediaVec{};
    mediaVec.push_back(std::move(firstMedium));
    mediaVec.push_back(std::move(secondMedium));

    const bool result{library->addMedia(std::move(mediaVec))};
    const auto mediaView = library->getMediaView();
    QCOMPARE(result, true);
    QCOMPARE(mediaView.size(), 2);
    QCOMPARE(mediaView.at(0)->title().get(), firstTitle);
    QCOMPARE(mediaView.at(1)->title().get(), secondTitle);
    QCOMPARE(mediaChangedSpy.count(), 1);
    QCOMPARE(mediaChangedSpy.at(0).at(0).value<std::vector<const Medium*>>(), mediaView);
}
void TestLibrary::testAddMediaInvalid() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};

    std::vector<std::unique_ptr<const Medium>> invalidMedia{};
    invalidMedia.push_back(std::make_unique<Book>());
    invalidMedia.push_back(nullptr);
    const bool result{lib->addMedia(std::move(invalidMedia))};
    QCOMPARE(result, false);
    QCOMPARE(lib->getMediaView().size(), 0);
    QCOMPARE(lib->getMediaView().size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 0);
}

void TestLibrary::testAddMedium() {
    const std::shared_ptr library{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{library.get(), &Library::mediaChanged};

    std::vector<std::unique_ptr<const Medium>> media{};
    library->addMedium(std::make_unique<Book>());
    library->addMedium(std::make_unique<Video>());

    auto view = library->getMediaView();
    QCOMPARE(view.size(), 2);
    QVERIFY(dynamic_cast<const Book*>(view.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Video*>(view.at(1)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 2);
    QCOMPARE(mediaChangedSpy.at(1).at(0).value<std::vector<const Medium*>>(), view);

    // Overwrite the vector that has just been set
    std::vector<std::unique_ptr<const Medium>> addedMedia{};
    library->addMedium(std::make_unique<Video>());
    library->addMedium(std::make_unique<Book>());
    library->addMedium(std::make_unique<Article>());

    view = library->getMediaView();
    QCOMPARE(view.size(), 5);
    QVERIFY(dynamic_cast<const Book*>(view.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Video*>(view.at(1)) != nullptr);
    QVERIFY(dynamic_cast<const Video*>(view.at(2)) != nullptr);
    QVERIFY(dynamic_cast<const Book*>(view.at(3)) != nullptr);
    QVERIFY(dynamic_cast<const Article*>(view.at(4)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 5);
    QCOMPARE(mediaChangedSpy.at(4).at(0).value<std::vector<const Medium*>>(), view);
}
void TestLibrary::testAddMediumInvalid() {
    const std::shared_ptr library{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{library.get(), &Library::mediaChanged};

    const bool res{library->addMedium(nullptr)};
    QCOMPARE(res, false);
    QCOMPARE(library->getMediaView().size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 0);
}

void TestLibrary::testReplaceMedium() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};

    lib->addMedium(std::make_unique<Book>());
    const auto mediumToReplacePtr{dynamic_cast<const Book*>(lib->getMediaView().at(0))};

    auto newMedium{std::make_unique<Video>()};
    const QString title{"This is totally new"};
    newMedium->title().set(title);
    const bool res{lib->replaceMedium(mediumToReplacePtr, std::move(newMedium))};
    const auto view{lib->getMediaView()};
    QCOMPARE(res, true);
    QCOMPARE(view.size(), 1);
    QCOMPARE(view.at(0)->title().get(), title);
    QCOMPARE(mediaChangedSpy.count(), 2);
    QCOMPARE(mediaChangedSpy.at(1).at(0).value<std::vector<const Medium*>>(), view);
}
void TestLibrary::testReplaceMediumInvalid() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};

    lib->addMedium(std::make_unique<Book>());
    const auto mediumToReplacePtr{dynamic_cast<const Book*>(lib->getMediaView().at(0))};

    const bool res{lib->replaceMedium(mediumToReplacePtr, nullptr)};
    QCOMPARE(res, false);
    QCOMPARE(lib->getMediaView().size(), 1);
    QCOMPARE(lib->getMediaView().at(0)->title().get(), std::nullopt);
    QCOMPARE(mediaChangedSpy.count(), 1);
}

void TestLibrary::testRemoveMedium() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};
    auto mediaView = lib->getMediaView();

    // Trying to remove something from an empty library should do nothing
    const auto mediumNotInLibrary{std::make_unique<Video>()};
    mediaView = lib->getMediaView();
    bool res{lib->removeMedium(mediumNotInLibrary.get())};
    QCOMPARE(res, false);
    QCOMPARE(mediaView.size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 0);

    lib->addMedium(std::make_unique<Book>());
    lib->addMedium(std::make_unique<Video>());
    lib->addMedium(std::make_unique<Article>());
    mediaView = lib->getMediaView();

    // remove a medium that is actually present
    QCOMPARE(mediaView.size(), 3);
    res = lib->removeMedium(mediaView.at(1));
    mediaView = lib->getMediaView();
    QCOMPARE(res, true);
    QCOMPARE(mediaView.size(), 2);
    QVERIFY(dynamic_cast<const Book*>(mediaView.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Article*>(mediaView.at(1)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 4);
    QCOMPARE(mediaChangedSpy.at(3).at(0).value<std::vector<const Medium*>>(), mediaView);

    // trying to remove nullptr should do nothing
    lib->removeMedium(nullptr);
    mediaView = lib->getMediaView();
    QCOMPARE(mediaView.size(), 2);
    QVERIFY(dynamic_cast<const Book*>(mediaView.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Article*>(mediaView.at(1)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 4);
    QCOMPARE(mediaChangedSpy.at(3).at(0).value<std::vector<const Medium*>>(), mediaView);

    // trying to remove a medium that is not present should do nothing
    lib->removeMedium(mediumNotInLibrary.get());
    mediaView = lib->getMediaView();
    QCOMPARE(mediaView.size(), 2);
    QVERIFY(dynamic_cast<const Book*>(mediaView.at(0)) != nullptr);
    QVERIFY(dynamic_cast<const Article*>(mediaView.at(1)) != nullptr);
    QCOMPARE(mediaChangedSpy.count(), 4);
    QCOMPARE(mediaChangedSpy.at(3).at(0).value<std::vector<const Medium*>>(), mediaView);
}

void TestLibrary::testClearMedia() {
    const std::shared_ptr lib{Library::getLibrary()};
    const QSignalSpy mediaChangedSpy{lib.get(), &Library::mediaChanged};
    auto mediaView = lib->getMediaView();

    // clearing an empty library should do nothing
    lib->clearMedia();
    mediaView = lib->getMediaView();
    QCOMPARE(mediaView.size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 0);

    // clearing a non-empty library should remove all elements from the media array
    lib->addMedium(std::make_unique<Book>());
    lib->addMedium(std::make_unique<Video>());
    lib->addMedium(std::make_unique<Article>());
    lib->clearMedia();
    mediaView = lib->getMediaView();
    QCOMPARE(mediaView.size(), 0);
    QCOMPARE(mediaChangedSpy.count(), 4);
    QCOMPARE(mediaChangedSpy.at(3).at(0).value<std::vector<const Medium*>>(), mediaView);
}

void TestLibrary::testGetAllTopics() {
    const std::shared_ptr lib{Library::getLibrary()};
    auto mediaView = lib->getMediaView();

    const QString compsci{"Computer science"};
    const QString physics{"Physics"};
    const QString math{"Math"};
    const QString philosophy{"Philosophy"};
    const QString painting{"Painting"};

    auto book{std::make_unique<Book>()};
    book->userData().topics().set(std::set{compsci, physics, math});
    lib->addMedium(std::move(book));

    auto video{std::make_unique<Video>()};
    video->userData().topics().set(std::set{math, philosophy, painting});
    lib->addMedium(std::move(video));

    auto article{std::make_unique<Article>()};
    lib->addMedium(std::move(article));

    const std::set expectedPresent{compsci, physics, math, philosophy, painting};

    QCOMPARE(lib->getAllTopics(), expectedPresent);
}
