#include "LibraryTopbar.h"

#include <QHBoxLayout>

namespace Gui::Components {

LibraryTopbar::LibraryTopbar(QWidget *parent)
    : QWidget{parent}
    , sidebarToggle{new QPushButton{}}
    , searchInput{new QLineEdit{}}
    , searchDebounceTimer{new QTimer{this}} {

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
    layout->addWidget(searchInput);
    setLayout(layout);
}
}