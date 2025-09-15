#ifndef LIBRARYTOPBAR_H
#define LIBRARYTOPBAR_H

#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace Gui::Components {

class LibraryTopbar : public QWidget {
    Q_OBJECT

  public:
    enum class MediumTypeViewModel : std::uint8_t {
        Article,
        Book,
        Video,
    };

  private:
    static constexpr int debounceMs{10};

    QPushButton *createMediumButton;
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
    void createMediumRequest(MediumTypeViewModel type);

  private slots:
    void onSearchInputChanged();
    void onDebounceTimeout();
    void onCreateMedium();

  private:
    void initCreateMediumButton();
    void initSidebarToggle();
    void initSearchInput();
    void initSearchDebounceTimer();
    void initLayout();
};

}

#endif