#include "ArticleDetailPageController.h"

namespace Gui::Components {

ArticleDetailPageController::ArticleDetailPageController(ArticleDetailPage *newView,
                                                         const Article *newOriginalArticle,
                                                         QObject *parent)
    : DetailPageController(parent)
    , view{newView}
    , originalArticle{newOriginalArticle}
    , editableArticle{*newOriginalArticle}
    , mediumSectionController{view->getMediumSection(), originalArticle, this}
    , articleSectionController{view->getArticleSection(), originalArticle, this}
    , userDataSectionController{view->getUserDataSection(), originalArticle, this} {

    view->setEditMode(false);

    connect(view, &DetailPage::saveRequested, this, &ArticleDetailPageController::onSave);
    connect(view, &DetailPage::editRequested, this, &ArticleDetailPageController::onEditRequested);
    connect(view, &DetailPage::discardChanges, this,
            &ArticleDetailPageController::onDiscardChanges);
    connect(view, &DetailPage::goBackToLibrary, this,
            &ArticleDetailPageController::goBackToLibraryRequest);
}

void ArticleDetailPageController::onSave() {
    mediumSectionController.applyChanges(editableArticle);
    articleSectionController.applyChanges(editableArticle);
    userDataSectionController.applyChanges(editableArticle);

    emit mediumEdited(editableArticle);
}

void ArticleDetailPageController::onEditRequested() {
    view->setEditMode(true);
}

void ArticleDetailPageController::onDiscardChanges() {
    view->setEditMode(false);
}

}