#include "ArticleDetailPage.h"

#include "components/medium-detail/detail-sections/ArticleDetailSection.h"

#include <QVBoxLayout>

namespace Gui::Components {

ArticleDetailPage::ArticleDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                                     const UserDataDetailSection::Dependencies &userDataDeps,
                                     const ArticleDetailSection::Dependencies &articleDeps,
                                     QWidget *parent)
    : DetailPage{parent}
    , mediumSection{new MediumDetailSection{mediumDeps, this}}
    , userDataSection{new UserDataDetailSection{userDataDeps, this}}
    , articleSection{new ArticleDetailSection{articleDeps, this}} {

    getContentLayout()->addWidget(mediumSection);
    getContentLayout()->addWidget(articleSection);
    getContentLayout()->addWidget(userDataSection);
    getContentLayout()->addStretch();
};

auto ArticleDetailPage::getMediumSection() const -> MediumDetailSection * {
    return mediumSection;
}

auto ArticleDetailPage::getUserDataSection() const -> UserDataDetailSection * {
    return userDataSection;
}

auto ArticleDetailPage::getArticleSection() const -> ArticleDetailSection * {
    return articleSection;
}

void ArticleDetailPage::setEditMode(bool isEditing) {
    DetailPage::setEditMode(isEditing);

    mediumSection->setEditMode(isEditing);
    userDataSection->setEditMode(isEditing);
    articleSection->setEditMode(isEditing);
}

}