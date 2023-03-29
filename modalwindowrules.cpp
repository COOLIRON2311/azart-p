#include "modalwindowrules.h"
#include "ui_modalwindowrules.h"

ModalWindowRules::ModalWindowRules(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModalWindowRules)
{
    ui->setupUi(this);
    ui->textBrowser->setText(text);
    setMinimumSize(size());
}

ModalWindowRules::~ModalWindowRules()
{
    delete ui;
}
