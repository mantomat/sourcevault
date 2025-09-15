#ifndef ARTICLECREATECONTROLLER_H
#define ARTICLECREATECONTROLLER_H

#include "components/medium-detail/ArticleDetailPage.h"
#include "components/medium-detail/CreateController.h"
#include "components/medium-detail/detail-sections/ArticleSectionController.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "model/Article.h"

using Core::Model::Article;

namespace Gui::Components {

class ArticleCreateController : public CreateController {

    ArticleDetailPage *view;

    Article newArticle;

    MediumSectionController mediumSectionController;
    ArticleSectionController articleSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~ArticleCreateController() override = default;

    ArticleCreateController(ArticleDetailPage *newView, QObject *parent);

    ArticleCreateController(const ArticleCreateController &) = delete;
    ArticleCreateController(ArticleCreateController &&) = delete;
    auto operator=(const ArticleCreateController &) -> ArticleCreateController & = delete;
    auto operator=(ArticleCreateController &&) -> ArticleCreateController & = delete;

  private slots:
    void onSave();
};

}

#endif