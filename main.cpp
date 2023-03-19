#include "mainwindow.h"
#include "modalwindowrd.h"
#include <QApplication>

inline void bootstrap(MainWindow& w)
{
    w.selfcontrol_screen();
   // w.loading_screen();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.setup();
//     bootstrap(w);
    w.service_menu_screen();

    w.rdwin.hide();

    return a.exec();
}
