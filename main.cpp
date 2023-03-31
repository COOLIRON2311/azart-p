#include "mainwindow.h"
#include "modalwindowrd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setup();
    //w.off_screen();
    //w.channel_list_screen();
    w.freq_plans_screen();
    w.rdwin.hide();
    return a.exec();
}
