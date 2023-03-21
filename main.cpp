#include "mainwindow.h"
#include "modalwindowrd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setup();
    w.selfcontrol_screen();
    w.rdwin.hide();
    return a.exec();
}
