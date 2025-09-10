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

    setupSorting();
    setupMediumTypeFilter();
    setupMinimumPriorityFilter();

    auto *mainLayout{new QVBoxLayout{this}};

    auto *sortingBox{new QGroupBox{"Sorting", this}};
    auto *sortingBoxLayout{new QFormLayout{sortingBox}};
    sortingBoxLayout->addRow("Sorting", sorting);

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

    mainLayout->addWidget(sortingBox);
    mainLayout->addWidget(filtersBox, 1);

    connect(minimumPriorityFilter, &QSlider::valueChanged, this,
            &LibrarySidebar::onMinimumPriorityFilterChanged);

    connect(sorting, &QComboBox::currentIndexChanged, this, &LibrarySidebar::stateChanged);
    connect(mediumTypeFilter, &QComboBox::currentIndexChanged, this, &LibrarySidebar::stateChanged);
    connect(minimumPriorityFilter, &QSlider::valueChanged, this, &LibrarySidebar::stateChanged);
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

void LibrarySidebar::setupSorting() const {
    sorting->addItem("Title (A-Z)", QVariant::fromValue(SortTypeOptions::TitleAsc));
    sorting->addItem("Title (Z-A)", QVariant::fromValue(SortTypeOptions::TitleDesc));
    sorting->addItem("Date Added (newest first)",
                     QVariant::fromValue(SortTypeOptions::DateAddedDesc));
    sorting->addItem("Date Added (oldest first)",
                     QVariant::fromValue(SortTypeOptions::DateAddedAsc));
}

void LibrarySidebar::setupMediumTypeFilter() const {
    mediumTypeFilter->addItem("Any", QVariant::fromValue(MediumTypeFilterOptions::NoFilter));
    mediumTypeFilter->addItem("Article", QVariant::fromValue(MediumTypeFilterOptions::Article));
    mediumTypeFilter->addItem("Book", QVariant::fromValue(MediumTypeFilterOptions::Book));
    mediumTypeFilter->addItem("Video", QVariant::fromValue(MediumTypeFilterOptions::Video));
}

void LibrarySidebar::setupMinimumPriorityFilter() const {
    minimumPriorityFilter->setRange(0, 5);
    minimumPriorityFilter->setValue(0);
    minimumPriorityFilter->setTickPosition(QSlider::TicksBelow);
    minimumPriorityFilter->setTracking(false);

    currentMinPriorityValue->setStyleSheet("font-size: 6pt;");
    currentMinPriorityValue->setText("Any");
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
        qDebug() << "Minumum priority filter unhandled value: " << value;
        break;
    }
}

auto LibrarySidebar::topicsFilterToData() const -> std::vector<QString> {
    std::vector<QString> selectedTopics;

    for (int i = 0; i < topicsFilter->count(); ++i) {
        QListWidgetItem *item = topicsFilter->item(i);
        if (item->checkState() == Qt::Checked) {
            selectedTopics.push_back(item->text());
        }
    }

    return selectedTopics;
}

}