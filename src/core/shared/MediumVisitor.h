#ifndef MEDIUMVISITOR_H
#define MEDIUMVISITOR_H

namespace Core::Model {
class Article;
class Book;
class Video;
}

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

namespace Core::Shared {

class MediumVisitor {
  public:
    virtual ~MediumVisitor() = default;
    MediumVisitor() = default;
    MediumVisitor(const MediumVisitor &) = default;
    MediumVisitor(MediumVisitor &&) = default;
    auto operator=(const MediumVisitor &) -> MediumVisitor & = default;
    auto operator=(MediumVisitor &&) -> MediumVisitor & = default;

    virtual void visit(const Book &book) = 0;
    virtual void visit(const Article &article) = 0;
    virtual void visit(const Video &video) = 0;
};

}

#endif