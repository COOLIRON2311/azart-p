#include "modalwindownorm.h"
#include "ui_modalwindownorm.h"

ModalWindowNorm::ModalWindowNorm(QWidget *parent) :
    QMainWindow(parent), n(0), elapsed(0, 0, 0),
    ui(new Ui::ModalWindowNorm)
{
    ui->setupUi(this);
    quartz.setInterval(1000);
    connect(&quartz, SIGNAL(timeout()), this, SLOT(each_sec()));
    auto s = size();
//    setMinimumSize(s);
//    setMaximumSize(s);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(1);

    ui->tableWidget->setHorizontalHeaderLabels(labels);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    set_text();
}

ModalWindowNorm::~ModalWindowNorm()
{
    delete ui;
}

inline void ModalWindowNorm::set_text()
{
    for (int i = 0; i < contents[n].size(); i++)
    {
        auto t = new QTableWidgetItem(contents[n][i]);
        if (i == 0 || i == 3 || i == 4)
            t->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0, i, t);
    }
}

void ModalWindowNorm::each_sec()
{
    elapsed = elapsed.addMSecs(1000);
    ui->watch->setText(elapsed.toString("mm:ss"));
}

void ModalWindowNorm::on_startb_pressed()
{
    elapsed = QTime(0, 0, 0);
    ui->watch->setText(elapsed.toString("mm:ss"));
    quartz.start();
}

void ModalWindowNorm::on_stopb_pressed()
{
    quartz.stop();
}


void ModalWindowNorm::on_prevb_clicked()
{
    if (n - 1 >= 0)
        n--;
    set_text();
}


void ModalWindowNorm::on_nextb_clicked()
{
    if (n + 1 < contents.size())
        n++;
    set_text();
}

