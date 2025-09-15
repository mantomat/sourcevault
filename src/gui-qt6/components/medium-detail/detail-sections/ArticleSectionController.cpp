#include "ArticleSectionController.h"

namespace Gui::Components {

ArticleSectionController::ArticleSectionController(ArticleDetailSection *newSection,
                                                   const Article *articleToDisplay, QObject *parent)
    : QObject{parent}
    , section{newSection} {
    populateSection(articleToDisplay);
}

void ArticleSectionController::applyChanges(Article &articleToModify) const {
    assert(section->isEverythingValid());

    const auto formData{section->getState()};

    if (formData.articleUrl.has_value()) {
        articleToModify.articleUrl().set(formData.articleUrl.value());
    } else {
        articleToModify.articleUrl().unset();
    }

    if (formData.sourceName.has_value()) {
        articleToModify.sourceName().set(formData.sourceName.value());
    } else {
        articleToModify.sourceName().unset();
    }

    if (formData.readTimeMinutes.has_value()) {
        articleToModify.readTimeMinutes().set(formData.readTimeMinutes.value());
    } else {
        articleToModify.readTimeMinutes().unset();
    }

    if (formData.publicationDate.has_value()) {
        articleToModify.publicationDate().set(formData.publicationDate.value());
    } else {
        articleToModify.publicationDate().unset();
    }
}

void ArticleSectionController::populateSection(const Article *articleToDisplay) {
    section->setState({.articleUrl = articleToDisplay->articleUrl().get(),
                       .sourceName = articleToDisplay->sourceName().get(),
                       .readTimeMinutes = articleToDisplay->readTimeMinutes().get(),
                       .publicationDate = articleToDisplay->publicationDate().get()});
}

}