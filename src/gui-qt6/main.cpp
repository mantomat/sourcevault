#include "AppController.h"

#include <QApplication>
#include <QPushButton>

using Gui::AppController;

auto main(int argc, char **argv) -> int {
    QApplication app{argc, argv};

    AppController appController;

    return QApplication::exec();
}
