#include "ArticleCreateController.h"

#include "components/medium-detail/CreateController.h"

namespace Gui::Components {

ArticleCreateController::ArticleCreateController(ArticleDetailPage *newView, QObject *parent)
    : CreateController{parent}
    , view{newView}
    , newArticle{Article::create("New Article").value()}
    , mediumSectionController{view->getMediumSection(), &newArticle, this}
    , articleSectionController{view->getArticleSection(), &newArticle, this}
    , userDataSectionController{view->getUserDataSection(), &newArticle, this} {
    view->setEditMode(true);

    connect(view, &DetailPage::saveRequested, this, &ArticleCreateController::onSave);
    connect(view, &DetailPage::discardChanges, this, &ArticleCreateController::creationCancelled);
}

void ArticleCreateController::onSave() {
    mediumSectionController.applyChanges(newArticle);
    articleSectionController.applyChanges(newArticle);
    userDataSectionController.applyChanges(newArticle);

    emit mediumCreated(newArticle);
}

}