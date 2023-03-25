#include "modalwindownorm.h"
#include "ui_modalwindownorm.h"

ModalWindowNorm::ModalWindowNorm(QWidget *parent) :
    QMainWindow(parent), n(0), elapsed(0, 0, 0),
    ui(new Ui::ModalWindowNorm)
{
    ui->setupUi(this);
    quartz.setInterval(1000);
    connect(&quartz, SIGNAL(timeout()), this, SLOT(each_sec()));
    double scale = QGuiApplication::primaryScreen()->logicalDotsPerInchY() / 96;
    QSize sz;
    if (abs(scale - 1.0) < 1e-5)
    {
        font = QFont("Segoe UI", 9, QFont::Normal);
        sz.setWidth(802);
        sz.setHeight(303);
    }
    else if (abs(scale - 1.25) < 1e-5)
    {
        font = QFont("Segoe UI", 8, QFont::Normal);
        sz.setWidth(902);
        sz.setHeight(403);
    }
    setMinimumSize(sz);
    setMaximumSize(sz);

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->horizontalHeader()->setFont(font);

    ui->tableWidget->setHorizontalHeaderLabels(labels);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setVisible(false);
    set_text();
    auto header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
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
        t->setFont(font);
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

