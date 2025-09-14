#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/library-page/LibraryPage.h"
#include "components/medium-detail/DetailPage.h"
#include "components/menubar/Menubar.h"

#include <QMainWindow>
#include <QStackedWidget>

using Gui::Components::DetailPage;
using Gui::Components::LibraryPage;
using Gui::Components::Menubar;

namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT

    Menubar *menubar;

    QStackedWidget *mainStack;
    LibraryPage *libraryPage;

    QLabel *actionFeedback;
    QTimer *actionFeedbackTimer;

    DetailPage *currentDetailPage;

  public:
    static constexpr int feedbackFadeSeconds{10};

    MainWindow();

    [[nodiscard]] auto getLibraryPage() const -> LibraryPage *;
    [[nodiscard]] auto getMenubar() const -> Menubar *;

  public slots:
    void setActionFeedback(const QString &feedback);
    void displayDetailPage(DetailPage *page);
    void closeDetailPage();

  private slots:
    void onActionFeedbackTimerTimeout();
};

}

#endif
