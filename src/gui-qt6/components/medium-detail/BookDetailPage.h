#ifndef BOOKDETAILPAGE_H
#define BOOKDETAILPAGE_H

#include "components/medium-detail/DetailPage.h"
#include "components/medium-detail/detail-sections/BookDetailSection.h"
#include "components/medium-detail/detail-sections/MediumDetailSection.h"
#include "components/medium-detail/detail-sections/UserDataDetailSection.h"
#include "components/thumbnail-renderer/ThumbnailRenderer.h"

#include <QWidget>

namespace Gui::Components {

class BookDetailPage : public DetailPage {

    ThumbnailRenderer *thumbnail;

    MediumDetailSection *mediumSection;
    UserDataDetailSection *userDataSection;
    BookDetailSection *bookSection;

  public:
    ~BookDetailPage() override = default;

    BookDetailPage(const MediumDetailSection::Dependencies &mediumDeps,
                   const UserDataDetailSection::Dependencies &userDataDeps,
                   const BookDetailSection::Dependencies &bookDeps, QWidget *parent);

    [[nodiscard]] auto getMediumSection() const -> MediumDetailSection *;
    [[nodiscard]] auto getUserDataSection() const -> UserDataDetailSection *;
    [[nodiscard]] auto getBookSection() const -> BookDetailSection *;

    BookDetailPage(const BookDetailPage &) = delete;
    BookDetailPage(BookDetailPage &&) = delete;
    auto operator=(const BookDetailPage &) -> BookDetailPage & = delete;
    auto operator=(BookDetailPage &&) -> BookDetailPage & = delete;

  public slots:
    void setEditMode(bool isEditing) override;

  private slots:
    void refreshThumbnail();
    void onStateChanged();
};

}

#endif