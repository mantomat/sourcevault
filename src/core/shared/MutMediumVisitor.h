#ifndef MUTMEDIUMVISITOR_H
#define MUTMEDIUMVISITOR_H

namespace Core::Model {
class Article;
class Book;
class Video;
}

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

namespace Core::Shared {

class MutMediumVisitor {
  public:
    virtual ~MutMediumVisitor() = default;
    MutMediumVisitor() = default;
    MutMediumVisitor(const MutMediumVisitor &) = default;
    MutMediumVisitor(MutMediumVisitor &&) = default;
    auto operator=(const MutMediumVisitor &) -> MutMediumVisitor & = default;
    auto operator=(MutMediumVisitor &&) -> MutMediumVisitor & = default;

    virtual void visit(Book &book) = 0;
    virtual void visit(Article &article) = 0;
    virtual void visit(Video &video) = 0;
};

}

#endif