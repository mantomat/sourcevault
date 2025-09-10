#ifndef LIBRARYTOPBAR_H
#define LIBRARYTOPBAR_H

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace Gui::Components {

class LibraryTopbar : public QWidget {
    Q_OBJECT

    static constexpr int debounceMs{10};

    QPushButton *sidebarToggle;

    QLineEdit *searchInput;
    QTimer *searchDebounceTimer;

  public:
    explicit LibraryTopbar(QWidget *parent);

    [[nodiscard]] auto isSidebarToggled() const -> bool;
    void setSidebarToggle(bool toggled);

    [[nodiscard]] auto getSearchQuery() const -> QString;

  signals:
    void searchQueryChanged();
    void sidebarToggled(bool showSidebar);

  private slots:
    void onSearchInputChanged();
    void onDebounceTimeout();

  private:
    void initSidebarToggle();
    void initSearchInput();
    void initSearchDebounceTimer();
    void initLayout();
};

}

#endif