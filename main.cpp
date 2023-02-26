#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.setup();
    //w.selfcontrol_screen();
    w.channel_list_screen();

    return a.exec();
}
