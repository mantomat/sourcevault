#include "MainWindow.h"

#include <QVBoxLayout>
#include <QWidget>

namespace Gui {

MainWindow::MainWindow()
    : menubar{new Menubar{this}}
    , mainStack{new QStackedWidget{this}}
    , libraryPage{new LibraryPage{this}}
    , actionFeedback{new QLabel{this}}
    , actionFeedbackTimer{new QTimer{this}}
    , currentDetailPage{nullptr} {

    actionFeedbackTimer->setSingleShot(true);
    actionFeedbackTimer->setInterval(feedbackFadeSeconds * 1000);
    connect(actionFeedbackTimer, &QTimer::timeout, this, &MainWindow::onActionFeedbackTimerTimeout);

    setMenuBar(menubar);

    QWidget *centralWidget{new QWidget{this}};
    QVBoxLayout *centralLayout{new QVBoxLayout{centralWidget}};

    centralLayout->addWidget(mainStack);
    centralLayout->addWidget(actionFeedback);

    mainStack->addWidget(libraryPage);

    setCentralWidget(centralWidget);

    resize(960, 540);
}

auto MainWindow::getLibraryPage() const -> LibraryPage * {
    return libraryPage;
}

auto MainWindow::getMenubar() const -> Menubar * {
    return menubar;
}

void MainWindow::setActionFeedback(const QString &feedback) {
    actionFeedback->setText(feedback);
    actionFeedbackTimer->start();
}

void MainWindow::displayDetailPage(DetailPage *page) {
    currentDetailPage = page;
    mainStack->addWidget(currentDetailPage);
    mainStack->setCurrentWidget(currentDetailPage);
}

void MainWindow::closeDetailPage() {
    if (currentDetailPage == nullptr) {
        return;
    }

    mainStack->setCurrentWidget(libraryPage);

    mainStack->removeWidget(currentDetailPage);
    currentDetailPage->deleteLater();
    currentDetailPage = nullptr;
}

void MainWindow::onActionFeedbackTimerTimeout() {
    actionFeedback->clear();
}

}
