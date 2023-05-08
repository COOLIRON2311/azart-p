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
    set_toc();
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

void ModalWindowRules::set_toc()
{
    // Оглавление
    toc = ui->menuBar->addMenu("&Оглавление");
    p1 = new QAction("Общие положения", this);
    toc->addAction(p1);
    connect(p1, &QAction::triggered, this, [this](){_scroll(0);});

    p2 = new QAction("Правила установления телефонной радиосвязи и ведение обмена", this);
    toc->addAction(p2);
    connect(p2, &QAction::triggered, this, [this](){_scroll(8320);});

    p3 = new QAction("Установление связи", this);
    toc->addAction(p3);
    connect(p3, &QAction::triggered, this, [this](){_scroll(8932);});

    p4 = new QAction("Передача радиограмм", this);
    toc->addAction(p4);
    connect(p4, &QAction::triggered, this, [this](){_scroll(9264);});

    p5 = new QAction("Передача сигналов", this);
    toc->addAction(p5);
    connect(p5, &QAction::triggered, this, [this](){_scroll(9770);});

    p6 = new QAction("Телефонные переговоры по радио", this);
    toc->addAction(p6);
    connect(p6, &QAction::triggered, this, [this](){_scroll(10597);});
}

void ModalWindowRules::_scroll(int pos)
{
    QTextCursor c = ui->textBrowser->textCursor();
    qDebug() << c.position();
    c.setPosition(pos);
    ui->textBrowser->setTextCursor(c);
}



