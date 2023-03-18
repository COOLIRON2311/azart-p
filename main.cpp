#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.setup();
    w.channel_list_screen();
    //w.selfcontrol_screen();

    return a.exec();
}
