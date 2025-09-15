#include "DetailPage.h"

#include <QPushButton>
#include <QVBoxLayout>

namespace Gui::Components {

DetailPage::~DetailPage() = default;

DetailPage::DetailPage(QWidget *parent)
    : QWidget{parent}
    , scrollArea{new QScrollArea}
    , contentLayout{new QVBoxLayout{}}
    , backDiscardStack{new QStackedWidget{this}}
    , goBackButton{new QPushButton{"Back", this}}
    , discardButton{new QPushButton{"Discard", this}}
    , editSaveStack{new QStackedWidget{this}}
    , editButton{new QPushButton{"Edit", this}}
    , saveButton{new QPushButton{"Save", this}} {

    auto *headerLayout{new QHBoxLayout{}};
    headerLayout->setContentsMargins(0, 0, 0, 0);

    backDiscardStack->addWidget(goBackButton);
    backDiscardStack->addWidget(discardButton);

    editSaveStack->addWidget(editButton);
    editSaveStack->addWidget(saveButton);

    goBackButton->setIcon(QIcon{":/assets/icons/arrow-left.png"});
    discardButton->setIcon(QIcon{":/assets/icons/arrow-left.png"});
    editButton->setIcon(QIcon{":/assets/icons/edit.png"});
    saveButton->setIcon(QIcon{":/assets/icons/save.png"});

    backDiscardStack->setMaximumWidth(100);
    editSaveStack->setMaximumWidth(100);

    headerLayout->addWidget(backDiscardStack);
    headerLayout->addStretch();
    headerLayout->addWidget(editSaveStack);

    auto *mainLayout{new QVBoxLayout(this)};
    mainLayout->addLayout(headerLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    auto *contentWidget{new QWidget{this}};
    contentWidget->setLayout(contentLayout);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea, 1);

    connect(goBackButton, &QPushButton::clicked, this, &DetailPage::goBackToLibrary);
    connect(editButton, &QPushButton::clicked, this, &DetailPage::editRequested);
    connect(discardButton, &QPushButton::clicked, this, &DetailPage::discardChanges);
    connect(saveButton, &QPushButton::clicked, this, &DetailPage::saveRequested);

    setEditMode(false);
}

auto DetailPage::getContentLayout() -> QVBoxLayout * {
    return contentLayout;
}

void DetailPage::setSaveButtonDisabled(bool disabled) {
    saveButton->setDisabled(disabled);
}

void DetailPage::setEditMode(bool isEditing) {
    backDiscardStack->setCurrentWidget(isEditing ? discardButton : goBackButton);
    editSaveStack->setCurrentWidget(isEditing ? saveButton : editButton);
}

}