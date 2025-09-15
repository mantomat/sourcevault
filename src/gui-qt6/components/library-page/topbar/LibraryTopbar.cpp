#include "LibraryTopbar.h"

#include <QAction>
#include <QHBoxLayout>
#include <QMenu>

namespace Gui::Components {

LibraryTopbar::LibraryTopbar(QWidget *parent)
    : QWidget{parent}
    , createMediumButton{new QPushButton{this}}
    , sidebarToggle{new QPushButton{}}
    , searchInput{new QLineEdit{}}
    , searchDebounceTimer{new QTimer{this}} {

    initCreateMediumButton();
    initSidebarToggle();
    initSearchInput();
    initSearchDebounceTimer();
    initLayout();
}

auto LibraryTopbar::isSidebarToggled() const -> bool {
    return sidebarToggle->isChecked();
}
void LibraryTopbar::setSidebarToggle(bool toggled) {
    sidebarToggle->setChecked(toggled);
}

auto LibraryTopbar::getSearchQuery() const -> QString {
    return searchInput->text();
}

void LibraryTopbar::onSearchInputChanged() {
    searchDebounceTimer->start();
}

void LibraryTopbar::onDebounceTimeout() {
    emit searchQueryChanged();
}

void LibraryTopbar::onCreateMedium() {
    QMenu contextMenu{this};
    QAction *articleAction{contextMenu.addAction("Article")};
    QAction *bookAction{contextMenu.addAction("Book")};
    QAction *videoAction{contextMenu.addAction("Video")};

    connect(articleAction, &QAction::triggered,
            [this]() { emit createMediumRequest(MediumTypeViewModel::Article); });
    connect(bookAction, &QAction::triggered,
            [this]() { emit createMediumRequest(MediumTypeViewModel::Book); });
    connect(videoAction, &QAction::triggered,
            [this]() { emit createMediumRequest(MediumTypeViewModel::Video); });

    contextMenu.exec(createMediumButton->mapToGlobal(QPoint(0, createMediumButton->height())));
}

void LibraryTopbar::initCreateMediumButton() {
    createMediumButton->setIcon(QIcon{":/assets/icons/plus.png"});
    connect(createMediumButton, &QPushButton::clicked, this, &LibraryTopbar::onCreateMedium);
}

void LibraryTopbar::initSidebarToggle() {
    sidebarToggle->setIcon(QIcon{":/assets/icons/menu.png"});
    sidebarToggle->setCheckable(true);
    connect(sidebarToggle, &QPushButton::toggled, this, &LibraryTopbar::sidebarToggled);
}

void LibraryTopbar::initSearchInput() {
    searchInput->setPlaceholderText("Search");
    searchInput->setClearButtonEnabled(true);
    connect(searchInput, &QLineEdit::textChanged, this, &LibraryTopbar::onSearchInputChanged);
}

void LibraryTopbar::initSearchDebounceTimer() {
    searchDebounceTimer->setSingleShot(true);
    searchDebounceTimer->setInterval(debounceMs);
    connect(searchDebounceTimer, &QTimer::timeout, this, &LibraryTopbar::onDebounceTimeout);
}

void LibraryTopbar::initLayout() {
    auto *layout{new QHBoxLayout{this}};
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(sidebarToggle);
    layout->addWidget(searchInput, 1);
    layout->addWidget(createMediumButton);
    setLayout(layout);
}

}