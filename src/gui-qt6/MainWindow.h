#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "components/library-page/LibraryPage.h"
#include "components/menubar/Menubar.h"

#include <QMainWindow>
#include <QStackedWidget>

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

  public:
    static constexpr int feedbackFadeSeconds{10};

    MainWindow();

    [[nodiscard]] auto getLibraryPage() const -> LibraryPage *;
    [[nodiscard]] auto getMenubar() const -> Menubar *;

  public slots:
    void setActionFeedback(const QString &feedback);

  private slots:
    void onActionFeedbackTimerTimeout();
};

}

#endif
