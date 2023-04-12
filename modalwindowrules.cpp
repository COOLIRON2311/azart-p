#include "modalwindowrules.h"
#include "ui_modalwindowrules.h"

ModalWindowRules::ModalWindowRules(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModalWindowRules)
{
    ui->setupUi(this);
    load_text();
    ui->textBrowser->setText(text);
    setMinimumSize(size());
    setMaximumWidth(size().width());
}

ModalWindowRules::~ModalWindowRules()
{
    delete ui;
}

void ModalWindowRules::load_text()
{
    QString fileName(":/resources/text.html");

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        text = "<h1>Ошибка загрузки справки</h1>";
    else
        text = file.readAll();
    file.close();
}

