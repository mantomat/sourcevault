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

    /* The following is common, shared data used along the test suite */
    const Book defaultBook{[] {
        auto b{Book::create("book title").value()};
        b.userData().topics().set({"t0", "t1"});
        return b;
    }()};
    const Video defaultVideo{[] {
        auto v{Video::create("video title").value()};
        v.userData().topics().set({"t1", "t2"});
        return v;
    }()};
    const Article defaultArticle{[] {
        auto a{Article::create("article title").value()};
        a.userData().topics().set({"t2", "t3"});
        return a;
    }()};

    /* A default library that contains `defaultBook`, `defaultArticle` and `defaultVideo` */
    const Library defaultLibrary{[this] {
        Library lib;

        lib.addMedium(std::make_unique<Book>(defaultBook));
        lib.addMedium(std::make_unique<Video>(defaultVideo));
        lib.addMedium(std::make_unique<Article>(defaultArticle));

        return lib;
    }()};

  private slots:
    void testCopyConstructor_data() const;
    static void testCopyConstructor();

    void testCopyAssignment_data() const;
    static void testCopyAssignment();

    void testSwap_data() const;
    static void testSwap();

    void testGetAllMedia_data() const;
    static void testGetAllMedia();

    void testGetAllTopics_data() const;
    static void testGetAllTopics();

    void testGetMedium_data() const;
    static void testGetMedium();

    void testMediaCount_data() const;
    static void testMediaCount();

    void testSetMedia_data() const;
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
};

#endif
