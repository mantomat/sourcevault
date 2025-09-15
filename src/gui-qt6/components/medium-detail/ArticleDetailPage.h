#ifndef ARTICLEDETAILPAGE_H
#define ARTICLEDETAILPAGE_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/detail-sections/ArticleDetailSection.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"

#include <QWidget>

namespace Gui::Components {

class ArticleDetailPage : public DetailPage {

    MediumDetailSection *mediumSection;
    UserDataDetailSection *userDataSection;
    ArticleDetailSection *articleSection;

  public:
    ~ArticleDetailPage() override = default;

    ArticleDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                      const UserDataDetailSection::Dependencies &userDataDeps,
                      const ArticleDetailSection::Dependencies &articleDeps, QWidget *parent);

    [[nodiscard]] auto getMediumSection() const -> MediumDetailSection *;
    [[nodiscard]] auto getUserDataSection() const -> UserDataDetailSection *;
    [[nodiscard]] auto getArticleSection() const -> ArticleDetailSection *;

    ArticleDetailPage(const ArticleDetailPage &) = delete;
    ArticleDetailPage(ArticleDetailPage &&) = delete;
    auto operator=(const ArticleDetailPage &) -> ArticleDetailPage & = delete;
    auto operator=(ArticleDetailPage &&) -> ArticleDetailPage & = delete;

  public slots:
    void setEditMode(bool isEditing) override;
};

}

#endif