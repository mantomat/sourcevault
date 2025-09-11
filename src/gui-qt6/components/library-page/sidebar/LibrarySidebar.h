#ifndef LIBRARYSIDEBAR_H
#define LIBRARYSIDEBAR_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QSlider>
#include <QWidget>
#include <set>
#include <unordered_set>

namespace Gui::Components {

class LibrarySidebar : public QWidget {
    Q_OBJECT

  public:
    /*
     * This section defines the public interface for the sidebar's state.
     *
     * These enums and structs serve as a "ViewModel," creating a clear and stable
     * contract between the LibrarySidebar (View) and the Controller.
     *
     * This decouples the View from the backend Model. The View only needs to know
     * how to construct a SidebarState object, while the Controller knows how to
     * interpret it to build a proper query.
     */

    enum class SortTypeOptions : std::uint8_t {
        TitleAsc,
        TitleDesc,
        DateAddedDesc,
        DateAddedAsc,
        PriorityAsc,
        PriorityDesc,
        Disabled
    };
    enum class MediumTypeFilterOptions : std::uint8_t { Article, Book, Video, NoFilter };
    enum class MinimumPriorityFilterOptions : std::uint8_t {
        NoFilter = 0,
        Min,
        Low,
        Mid,
        High,
        Max,
    };

    struct SidebarState {
        SortTypeOptions sort;
        MediumTypeFilterOptions mediumTypeFilter;
        MinimumPriorityFilterOptions minimumPriorityFilter;
        std::unordered_set<QString> topicsFilter;
        bool onlyFavoriteFilter;
    };

  private:
    QComboBox *sorting;

    QComboBox *mediumTypeFilter;
    QSlider *minimumPriorityFilter;
    QLabel *currentMinPriorityValue;
    QCheckBox *onlyFavoriteFilter;
    QListWidget *topicsFilter;

  public:
    LibrarySidebar(QWidget *parent);

    void disableSorting() const;
    void enableSorting() const;

    void setAvailableTopics(const std::set<QString> &topics);

    [[nodiscard]] auto getState() const -> SidebarState;

  signals:
    void stateChanged();

  private:
    void initSorting() const;
    void initMediumTypeFilter() const;
    void initMinimumPriorityFilter();

    auto initSortingBox() -> QGroupBox *;
    auto initFiltersBox() -> QGroupBox *;
    void initMainLayout(QGroupBox *sortingBox, QGroupBox *filtersBox);

    [[nodiscard]] auto topicsFilterToData() const -> std::unordered_set<QString>;

  private slots:
    void onMinimumPriorityFilterChanged(int value);
};

}

#endif