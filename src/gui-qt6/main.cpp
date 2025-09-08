#include <QApplication>
#include <QPushButton>

auto main(int argc, char **argv) -> int {
    QApplication app(argc, argv);

    QPushButton button("Hello world !");
    button.show();

    return QApplication::exec();
}
