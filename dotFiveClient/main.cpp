#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(
                ":/fonts/fontawesome.ttf");
    QFontDatabase::addApplicationFont(
                ":/fonts/menlo.ttf");
    QFontDatabase::addApplicationFont(
                ":/fonts/menlo-bold.ttf");

    MainWindow w;
    w.show();

    return a.exec();
}
