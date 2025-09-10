#include "LibraryTopbar.h"

#include <QHBoxLayout>

namespace Gui::Components {

LibraryTopbar::LibraryTopbar(QWidget *parent)
    : QWidget{parent}
    , sidebarToggle{new QPushButton{}}
    , searchInput{new QLineEdit{}}
    , searchDebounceTimer{new QTimer{this}} {

    auto *layout{new QHBoxLayout{this}};

    sidebarToggle->setIcon(QIcon{":/assets/icons/menu.png"});
    sidebarToggle->setCheckable(true);
    connect(sidebarToggle, &QPushButton::toggled, this, &LibraryTopbar::sidebarToggled);

    layout->addWidget(sidebarToggle);

    searchInput->setPlaceholderText("Search");
    searchInput->setClearButtonEnabled(true);
    layout->addWidget(searchInput);

    setLayout(layout);

    searchDebounceTimer->setSingleShot(true);
    searchDebounceTimer->setInterval(300);

    connect(searchInput, &QLineEdit::textChanged, this, &LibraryTopbar::onSearchInputChanged);
    connect(searchDebounceTimer, &QTimer::timeout, this, &LibraryTopbar::onDebounceTimeout);
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

void LibraryTopbar::onSearchInputChanged(const QString & /*unused*/) {
    searchDebounceTimer->start();
}

void LibraryTopbar::onDebounceTimeout() {
    emit searchQueryChanged();
}

}