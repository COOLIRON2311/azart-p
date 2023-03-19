#include "modalwindow3d.h"
#include "ui_modalwindow3d.h"

ModalWindow3D::ModalWindow3D(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModalWindow3D)
{
    ui->setupUi(this);
}

ModalWindow3D::~ModalWindow3D()
{
    delete ui;
}
