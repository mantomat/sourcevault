#ifndef ARTICLESECTIONCONTROLLER_H
#define ARTICLESECTIONCONTROLLER_H

#include "ArticleDetailSection.h"
#include "model/Article.h"

#include <QObject>

using Core::Model::Article;

namespace Gui::Components {

class ArticleSectionController : public QObject {
    Q_OBJECT

    ArticleDetailSection *section;

  public:
    ~ArticleSectionController() override = default;

    ArticleSectionController(ArticleDetailSection *newSection, const Article *articleToDisplay,
                             QObject *parent);

    ArticleSectionController(const ArticleSectionController &) = delete;
    ArticleSectionController(ArticleSectionController &&) = delete;
    auto operator=(const ArticleSectionController &) -> ArticleSectionController & = delete;
    auto operator=(ArticleSectionController &&) -> ArticleSectionController & = delete;

    void applyChanges(Article &articleToModify) const;

  private:
    void populateSection(const Article *articleToDisplay);
};

}

#endif