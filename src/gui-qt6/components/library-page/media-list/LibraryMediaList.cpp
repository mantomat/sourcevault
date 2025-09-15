#include "LibraryMediaList.h"

#include "LibraryMediumCard.h"

#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

namespace Gui::Components {

LibraryMediaList::LibraryMediaList(QWidget *parent)
    : QWidget{parent}
    , mediaList{new QListWidget{this}} {

    initLayout();
    initMediaList();

    connect(mediaList, &QListWidget::itemClicked, this, &LibraryMediaList::onItemClicked);
    connect(mediaList, &QListWidget::customContextMenuRequested, this,
            &LibraryMediaList::onContextMenuRequested);
}

void LibraryMediaList::setMedia(const std::vector<LibraryMediumCard::MediumCardViewModel> &media) {
    mediaList->clear();

    for (const auto &mediumData : media) {
        auto *card{new LibraryMediumCard(mediumData, this)};

        auto *item{new QListWidgetItem(mediaList)};

        item->setSizeHint(card->sizeHint());
        item->setData(Qt::UserRole, QVariant::fromValue(mediumData.id));

        mediaList->addItem(item);
        mediaList->setItemWidget(item, card);
    }
}

void LibraryMediaList::removeFromList(const QUuid &id) {
    for (int i = 0; i < mediaList->count(); ++i) {
        const QListWidgetItem *item = mediaList->item(i);
        const QUuid itemId = item->data(Qt::UserRole).toUuid();
        if (itemId == id) {
            delete mediaList->takeItem(i);
            break;
        }
    }
}

void LibraryMediaList::onItemClicked(QListWidgetItem *item) {
    emit mediumDetailRequest(item->data(Qt::UserRole).toUuid());
}

void LibraryMediaList::onContextMenuRequested(const QPoint &pos) {
    QListWidgetItem *item = mediaList->itemAt(pos);
    if (item == nullptr) {
        return;
    }

    QMenu contextMenu;
    QAction *removeAction = contextMenu.addAction("Remove");

    connect(removeAction, &QAction::triggered,
            [this, item]() { emit mediumDeleteRequest(item->data(Qt::UserRole).toUuid()); });

    contextMenu.exec(mediaList->mapToGlobal(pos));
}

void LibraryMediaList::initLayout() {
    auto *layout{new QVBoxLayout(this)};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mediaList);
}

void LibraryMediaList::initMediaList() {
    mediaList->setViewMode(QListView::IconMode);
    mediaList->setFlow(QListView::LeftToRight);
    mediaList->setWrapping(true);
    mediaList->setResizeMode(QListView::Adjust);
    mediaList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mediaList->setDragDropMode(QAbstractItemView::NoDragDrop);
    mediaList->setContextMenuPolicy(Qt::CustomContextMenu);
    mediaList->setStyleSheet("QListWidget::item { border-radius: 5px; } "
                             "QListWidget::item:hover { background-color: #e0e0e0; }");
}

}