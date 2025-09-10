#include "LibraryPage.h"

#include <QLabel>
#include <QVBoxLayout>

namespace Gui::Components {

LibraryPage::LibraryPage(QWidget *parent)
    : QWidget{parent}
    , splitter{new QSplitter{this}}
    , topbar{new LibraryTopbar{this}}
    , mediaList{new LibraryMediaList{this}}
    , sidebar{new LibrarySidebar{this}} {

    auto *pageLayout{new QVBoxLayout{this}};
    pageLayout->setContentsMargins(0, 0, 0, 0);

    splitter->addWidget(sidebar);

    auto *rightPanel{new QWidget{this}};
    auto *rightPanelLayout{new QVBoxLayout{}};

    rightPanelLayout->addWidget(topbar);

    rightPanelLayout->addWidget(mediaList, 1);

    rightPanel->setLayout(rightPanelLayout);
    splitter->addWidget(rightPanel);

    splitter->setSizes({0, 1});

    pageLayout->addWidget(splitter);

    connect(topbar, &LibraryTopbar::sidebarToggled, this, &LibraryPage::onSidebarToggled);
    connect(splitter, &QSplitter::splitterMoved, this, &LibraryPage::onSplitterMoved);
}

auto LibraryPage::getSidebar() const -> LibrarySidebar * {
    return sidebar;
}
auto LibraryPage::getTopbar() const -> LibraryTopbar * {
    return topbar;
}
auto LibraryPage::getMediaList() const -> LibraryMediaList * {
    return mediaList;
}

void LibraryPage::onSidebarToggled(bool showSidebar) {
    const auto currentSizes = splitter->sizes();
    const int currentSidebarWidth = currentSizes.first();
    const int currentRightPanelWidth = currentSizes.last();

    const int totalWidth{currentSidebarWidth + currentRightPanelWidth};

    if (showSidebar) {
        splitter->setSizes({sidebarWidth, totalWidth - sidebarWidth});
        return;
    }

    if (currentSidebarWidth > 0) {
        sidebarWidth = currentSidebarWidth;
    }

    splitter->setSizes({0, totalWidth});
}

void LibraryPage::onSplitterMoved(int pos, int index) {
    assert(index == 1);

    if (pos == 0 && topbar->isSidebarToggled()) {
        topbar->setSidebarToggle(false);
    }
    if (pos > 0 && !topbar->isSidebarToggled()) {
        topbar->setSidebarToggle(true);
    }
}

}