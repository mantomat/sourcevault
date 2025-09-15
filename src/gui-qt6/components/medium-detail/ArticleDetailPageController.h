#ifndef ARTICLEDETAILPAGECONTROLLER_H
#define ARTICLEDETAILPAGECONTROLLER_H

#include "components/medium-detail/ArticleDetailPage.h"
#include "components/medium-detail/DetailPageController.h"
#include "components/medium-detail/detail-sections/ArticleSectionController.h"
#include "components/medium-detail/detail-sections/MediumSectionController.h"
#include "components/medium-detail/detail-sections/UserDataSectionController.h"
#include "model/Article.h"

using Core::Model::Article;

namespace Gui::Components {

class ArticleDetailPageController : public DetailPageController {
    Q_OBJECT

    ArticleDetailPage *view;

    const Article *originalArticle;
    Article editableArticle;

    MediumSectionController mediumSectionController;
    ArticleSectionController articleSectionController;
    UserDataSectionController userDataSectionController;

  public:
    ~ArticleDetailPageController() override = default;

    ArticleDetailPageController(ArticleDetailPage *newView, const Article *newOriginalArticle,
                                QObject *parent);

    ArticleDetailPageController(const ArticleDetailPageController &) = delete;
    ArticleDetailPageController(ArticleDetailPageController &&) = delete;
    auto operator=(const ArticleDetailPageController &) -> ArticleDetailPageController & = delete;
    auto operator=(ArticleDetailPageController &&) -> ArticleDetailPageController & = delete;

  private slots:
    void onSave();
    void onEditRequested();
    void onDiscardChanges();
};

}

#endif