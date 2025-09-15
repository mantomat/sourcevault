#ifndef DETAILPAGE_H
#define DETAILPAGE_H

#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

namespace Gui::Components {

class DetailPage : public QWidget {
    Q_OBJECT

    QScrollArea *scrollArea;
    QVBoxLayout *contentLayout;

    QStackedWidget *backDiscardStack;
    QPushButton *goBackButton;
    QPushButton *discardButton;

    QStackedWidget *editSaveStack;
    QPushButton *editButton;
    QPushButton *saveButton;

  public:
    ~DetailPage() override = 0;

    DetailPage(QWidget *parent);

    DetailPage(const DetailPage &) = delete;
    DetailPage(DetailPage &&) = delete;
    auto operator=(const DetailPage &) -> DetailPage & = delete;
    auto operator=(DetailPage &&) -> DetailPage & = delete;

  protected:
    [[nodiscard]] auto getContentLayout() -> QVBoxLayout *;
    void setSaveButtonDisabled(bool disabled);

  public slots:
    virtual void setEditMode(bool isEditing);

  signals:
    void goBackToLibrary();
    void editRequested();
    void discardChanges();
    void saveRequested();
};

}

#endif