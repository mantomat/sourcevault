#include "LibrarySidebar.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <qcheckbox.h>

namespace Gui::Components {

LibrarySidebar::LibrarySidebar(QWidget *parent)
    : QWidget{parent}
    , sorting{new QComboBox{this}}
    , mediumTypeFilter{new QComboBox{this}}
    , minimumPriorityFilter{new QSlider{Qt::Horizontal, this}}
    , currentMinPriorityValue{new QLabel{this}}
    , onlyFavoriteFilter{new QCheckBox{this}}
    , topicsFilter{new QListWidget{this}} {

    initSorting();
    initMediumTypeFilter();
    initMinimumPriorityFilter();

    initMainLayout(initSortingBox(), initFiltersBox());

    connect(onlyFavoriteFilter, &QCheckBox::stateChanged, this, &LibrarySidebar::stateChanged);
    connect(topicsFilter, &QListWidget::itemChanged, this, &LibrarySidebar::stateChanged);
}

void LibrarySidebar::disableSorting() const {
    sorting->setDisabled(true);
}

void LibrarySidebar::enableSorting() const {
    sorting->setDisabled(false);
}

void LibrarySidebar::setAvailableTopics(const std::set<QString> &topics) {
    topicsFilter->clear();

    for (const QString &topic : topics) {
        auto *item{new QListWidgetItem{topic, topicsFilter}};
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

auto LibrarySidebar::getState() const -> SidebarState {
    return SidebarState{
        .sort = sorting->isEnabled() ? sorting->currentData().value<SortTypeOptions>()
                                     : SortTypeOptions::Disabled,
        .mediumTypeFilter = mediumTypeFilter->currentData().value<MediumTypeFilterOptions>(),
        .minimumPriorityFilter =
            static_cast<MinimumPriorityFilterOptions>(minimumPriorityFilter->value()),
        .topicsFilter = topicsFilterToData(),
        .onlyFavoriteFilter = onlyFavoriteFilter->isChecked(),
    };
}

void LibrarySidebar::initSorting() const {
    sorting->addItem("Title (A-z)", QVariant::fromValue(SortTypeOptions::TitleAsc));
    sorting->addItem("Title (z-A)", QVariant::fromValue(SortTypeOptions::TitleDesc));
    sorting->addItem("Date Added (newest first)",
                     QVariant::fromValue(SortTypeOptions::DateAddedDesc));
    sorting->addItem("Date Added (oldest first)",
                     QVariant::fromValue(SortTypeOptions::DateAddedAsc));
    sorting->addItem("Priority (highest first)", QVariant::fromValue(SortTypeOptions::PriorityAsc));
    sorting->addItem("Priority (lowest first)", QVariant::fromValue(SortTypeOptions::PriorityDesc));

    connect(sorting, &QComboBox::currentIndexChanged, this, &LibrarySidebar::stateChanged);
}

void LibrarySidebar::initMediumTypeFilter() const {
    mediumTypeFilter->addItem("Any", QVariant::fromValue(MediumTypeFilterOptions::NoFilter));
    mediumTypeFilter->addItem("Article", QVariant::fromValue(MediumTypeFilterOptions::Article));
    mediumTypeFilter->addItem("Book", QVariant::fromValue(MediumTypeFilterOptions::Book));
    mediumTypeFilter->addItem("Video", QVariant::fromValue(MediumTypeFilterOptions::Video));

    connect(mediumTypeFilter, &QComboBox::currentIndexChanged, this, &LibrarySidebar::stateChanged);
}

void LibrarySidebar::initMinimumPriorityFilter() {
    minimumPriorityFilter->setRange(0, 5);
    minimumPriorityFilter->setTickPosition(QSlider::TicksBelow);
    minimumPriorityFilter->setTracking(false);

    currentMinPriorityValue->setStyleSheet("font-size: 6pt;");

    minimumPriorityFilter->setValue(0);
    onMinimumPriorityFilterChanged(0);

    connect(minimumPriorityFilter, &QSlider::valueChanged, this,
            &LibrarySidebar::onMinimumPriorityFilterChanged);
    connect(minimumPriorityFilter, &QSlider::valueChanged, this, &LibrarySidebar::stateChanged);
}

void LibrarySidebar::onMinimumPriorityFilterChanged(int value) {
    switch (value) {
    case 0:
        currentMinPriorityValue->setText("Any");
        break;
    case 1:
        currentMinPriorityValue->setText("Min");
        break;
    case 2:
        currentMinPriorityValue->setText("Low");
        break;
    case 3:
        currentMinPriorityValue->setText("Mid");
        break;
    case 4:
        currentMinPriorityValue->setText("High");
        break;
    case 5:
        currentMinPriorityValue->setText("Max");
        break;
    default:
        break;
    }
}

auto LibrarySidebar::topicsFilterToData() const -> std::unordered_set<QString> {
    std::unordered_set<QString> selectedTopics;

    for (int i = 0; i < topicsFilter->count(); ++i) {
        QListWidgetItem *item = topicsFilter->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedTopics.insert(item->text());
        }
    }

    return selectedTopics;
}

auto LibrarySidebar::initSortingBox() -> QGroupBox * {
    auto *sortingBox{new QGroupBox{"Sorting", this}};
    auto *sortingBoxLayout{new QFormLayout{sortingBox}};
    sortingBoxLayout->addRow("Sorting", sorting);

    return sortingBox;
}

auto LibrarySidebar::initFiltersBox() -> QGroupBox * {
    auto *filtersBox{new QGroupBox{"Filters", this}};
    auto *filtersBoxLayout{new QFormLayout{filtersBox}};

    auto *minimumPriorityBox{new QWidget{this}};
    auto *minimumPriorityBoxLayout{new QVBoxLayout{minimumPriorityBox}};
    minimumPriorityBoxLayout->addWidget(minimumPriorityFilter);
    minimumPriorityBoxLayout->addWidget(currentMinPriorityValue);
    minimumPriorityBox->setLayout(minimumPriorityBoxLayout);

    topicsFilter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    filtersBoxLayout->addRow("Only Favorites", onlyFavoriteFilter);
    filtersBoxLayout->addRow("Type", mediumTypeFilter);
    filtersBoxLayout->addRow("Min Priority", minimumPriorityBox);
    filtersBoxLayout->addRow("Topics", topicsFilter);

    return filtersBox;
}

void LibrarySidebar::initMainLayout(QGroupBox *sortingBox, QGroupBox *filtersBox) {
    auto *mainLayout{new QVBoxLayout{this}};

    mainLayout->addWidget(sortingBox);
    mainLayout->addWidget(filtersBox, 1);
}

}