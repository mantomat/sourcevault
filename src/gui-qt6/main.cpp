#include "AppController.h"
#include "MainWindow.h"

#include <QApplication>
#include <QPushButton>

using Gui::AppController;
using Gui::MainWindow;

auto main(int argc, char **argv) -> int {
    QApplication app{argc, argv};

    MainWindow mainWindow{};
    AppController appController{&mainWindow};
    mainWindow.show();

    return QApplication::exec();
}
