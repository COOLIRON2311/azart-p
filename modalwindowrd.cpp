#include "modalwindowrd.h"
#include "ui_modalwindowrd.h"

ModalWindowRD::ModalWindowRD(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModalWindowRD)
{
    ui->setupUi(this);
    auto blank = QPixmap(":/resources/rdblank.png");
    resize(blank.size() / 2);
    setMinimumSize(blank.size() / 4);
}

ModalWindowRD::~ModalWindowRD()
{
    delete ui;
}
