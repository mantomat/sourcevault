#include "MediumTypeVisitor.h"

#include "model/Article.h"
#include "model/Book.h"
#include "model/Video.h"

using Core::Model::Article;
using Core::Model::Book;
using Core::Model::Video;

namespace Core::Shared {

MediumTypeVisitor::MediumTypeVisitor(std::unordered_set<std::type_index> newTargetTypes)
    : targetTypes{std::move(newTargetTypes)} {}

auto MediumTypeVisitor::hasMatched() const -> bool {
    return match;
}

void MediumTypeVisitor::visit(const Book &book) {
    match = targetTypes.contains(typeid(book));
}

void MediumTypeVisitor::visit(const Article &article) {
    match = targetTypes.contains(typeid(article));
}

void MediumTypeVisitor::visit(const Video &video) {
    match = targetTypes.contains(typeid(video));
}

}