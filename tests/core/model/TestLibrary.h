#ifndef TESTLIBRARY_H
#define TESTLIBRARY_H

#include "model/Article.h"
#include "model/Book.h"
#include "model/Library.h"
#include "model/Video.h"

#include <QObject>

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Library;
using Core::Model::Medium;
using Core::Model::Video;

class TestLibrary : public QObject {
    Q_OBJECT

  private slots:
    void testGetAllMedia() const;

    void testGetAllTopics() const;

    void testGetMedium_data() const;
    void testGetMedium() const;

    void testMediaCount() const;

    static void testSetMedia_data();
    static void testSetMedia();

    void testMerge_data() const;
    static void testMerge();

    void testAddMedium_data() const;
    static void testAddMedium();

    void testReplaceMedium_data() const;
    static void testReplaceMedium();

    void testRemoveMedium_data() const;
    static void testRemoveMedium();

    void testClear_data() const;
    static void testClear();

  private:
    /* The following is common, shared data used along the test suite */

    const Book book{[] {
        auto b{Book::create("book title").value()};
        b.userData().topics().set({"t0", "t1"});
        return b;
    }()};
    const Video video{[] {
        auto v{Video::create("video title").value()};
        v.userData().topics().set({"t1", "t2"});
        return v;
    }()};
    const Article article{[] {
        auto a{Article::create("article title").value()};
        a.userData().topics().set({"t2", "t3"});
        return a;
    }()};

    const std::set<QUuid> expectedIds{book.id(), video.id(), article.id()};
    std::set<QString> expectedTopics{"t0", "t1", "t2", "t3"};
    size_t expectedCount{3};

    void populateLib(Library& libToPopulate) const;
};

#endif
