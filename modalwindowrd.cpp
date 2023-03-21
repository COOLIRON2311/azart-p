#include "modalwindowrd.h"
#include "ui_modalwindowrd.h"

ModalWindowRD::ModalWindowRD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModalWindowRD)
{
    ui->setupUi(this);
    auto blank = QPixmap(":/resources/rdblank.png");
    setMinimumSize(blank.size());
    setMaximumSize(blank.size());
}

ModalWindowRD::~ModalWindowRD()
{
    delete ui;
}
