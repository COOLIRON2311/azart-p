#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QtMultimedia>
#include <iostream>
#include <QScrollBar>
#include <QLabel>
/*

PA - pay attention
TODO - need to do
Warning - warning (lmao)
    ::DS - will be deprecated soon (WDS)
    ::D - deprecated (WD)

*/

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

void MainWindow::change_global_time()
{
    QTime time = QTime::currentTime();

    QString hm, s;
    hm.sprintf("%02d:%02d", time.hour(), time.minute());
    s.sprintf("%02d", time.second());

    ui->hours_minutes->setText(hm);
    ui->seconds->setText(":" + s);

    ui->hours_minutes_2->setText(hm);
    ui->seconds_2->setText(":" + s);

    ui->data_label->setText(QDate::currentDate().toString("dd.MM.yyyy"));
}

QPoint MainWindow::global_pos(QWidget* w){
    while(w != this){
        return global_pos(w->parentWidget()) + w->pos();
    }
    return QPoint(0, 0);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    time_timer.setInterval(1000);
    time_timer.connect(&time_timer, &QTimer::timeout, this, &MainWindow::change_global_time);
    time_timer.start();

    ui->hours_minutes->setText("");
    ui->seconds->setText("");

    menu_list_item[0] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Шумоподавление");
    menu_list_item[1] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Громкость");
    menu_list_item[2] = new QListWidgetItem(QIcon(":/resources/nav32.png"), "Навигация");
    menu_list_item[3] = new QListWidgetItem(QIcon(":/resources/inbox32.png"), "Принятые сообщения");
    menu_list_item[4] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Настройки");
    menu_list_item[5] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Сервисное меню");
    menu_list_item[6] = new QListWidgetItem(QIcon(":/resources/battery32.png"), "Батарея");

    for(QListWidgetItem* item : menu_list_item){
        ui->menu_list->addItem(item);
    }

    // PA
    //selected_items["menu_list"] = menu_list_item[0];
    ui->menu_list->setCurrentItem(menu_list_item[0]);

    service_menu_list_item[0] = new QListWidgetItem(QIcon(""), "RS-485. Прием.");
    service_menu_list_item[1] = new QListWidgetItem(QIcon(""), "RS-485. Передача.");
    service_menu_list_item[2] = new QListWidgetItem(QIcon(""), "USB. Прием.");
    service_menu_list_item[3] = new QListWidgetItem(QIcon(""), "Блютус. Прием.");
    service_menu_list_item[4] = new QListWidgetItem(QIcon(""), "Блютус. Передача.");
    service_menu_list_item[5] = new QListWidgetItem(QIcon(""), "ИК. Прием.");
    service_menu_list_item[6] = new QListWidgetItem(QIcon(""), "ИК. Передача.");
    service_menu_list_item[7] = new QListWidgetItem(QIcon(""), "Редактор данных");
    service_menu_list_item[8] = new QListWidgetItem(QIcon(""), "Версия ПО");

    for(QListWidgetItem* item : service_menu_list_item){
        ui->service_menu_list->addItem(item);
    }

    // PA
    //selected_items["service_menu_list"] = service_menu_list_item[0];
    ui->service_menu_list->setCurrentItem(service_menu_list_item[0]);

    data_editor_list_item[0] = new QListWidgetItem(QIcon(""), "Общие параметры");
    data_editor_list_item[1] = new QListWidgetItem(QIcon(""), "Направления");
    data_editor_list_item[2] = new QListWidgetItem(QIcon(""), "Каналы");
    data_editor_list_item[3] = new QListWidgetItem(QIcon(""), "Списки сканирования");
    data_editor_list_item[4] = new QListWidgetItem(QIcon(""), "Частотные планы ППРЧ");
    data_editor_list_item[5] = new QListWidgetItem(QIcon(""), "Ключи");
    data_editor_list_item[6] = new QListWidgetItem(QIcon(""), "Режимы работы");
    data_editor_list_item[7] = new QListWidgetItem(QIcon(""), "Изменение пароля");
    data_editor_list_item[8] = new QListWidgetItem(QIcon(""), "...");

    for(QListWidgetItem* item : data_editor_list_item){
        ui->data_editor_list->addItem(item);
    }

    // PA
    //selected_items["data_editor_list"] = data_editor_list_item[0];
    ui->data_editor_list->setCurrentItem(data_editor_list_item[0]);

    //
    channel_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    channel_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    channel_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : channel_popup_menu_list_item){
        ui->channel_popup_menu_list->addItem(item);
    }

    // PA
    //selected_items["channel_popup_menu_list"] = channel_popup_menu_list_item[0];
    ui->channel_popup_menu_list->setCurrentItem(channel_popup_menu_list_item[0]);

    //ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

    //
    direction_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    direction_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    direction_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : direction_popup_menu_list_item){
        ui->direction_popup_menu_list->addItem(item);
    }
    // PA
    //selected_items["direction_popup_menu_list"] = direction_popup_menu_list_item[0];
    ui->direction_popup_menu_list->setCurrentItem(direction_popup_menu_list_item[0]);

    //ui->direction_popup_menu->setEnabled(false);
    ui->direction_popup_menu->setVisible(false);

    //                0       1      2      3      4       5        6        7      8
    channel_types = {"none", "dmo", "tmo", "vpd", "am25", "chm25", "chm50", "obp", "fm"};
    //                0       1      2      3      4       5        6        7      8
    direction_types = {"none_d", "dmo_d", "tmo_d", "vpd_d", "am25_d", "chm25_d", "chm50_d", "obp_d", "fm_d"};
    //                         0
    editor_fields["none"] = { "type" };
    curr_editor_field["none"] = 0;
    //                         0
    editor_fields["none_d"] = { "type" };
    curr_editor_field["none_d"] = 0;
    //                          0       1      2        3       4           5           6        7
    editor_fields["chm25"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "ctcss", "name" };
    curr_editor_field["chm25"] = 0;
    //                          0       1      2        3       4           5           6        7
    editor_fields["chm50"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "ctcss", "name" };
    curr_editor_field["chm50"] = 0;
    //                            0          1      2       3       4             5       6
    editor_fields["chm25_d"] = { "channel", "prd", "tone", "scan", "economizer", "name", "background" };
    curr_editor_field["chm25_d"] = 0;
    curr_editor_field["chm50_d"] = 0;

    //channel_editor_state
    ui->channel_editor_state->setProperty("chosen", 0);
    channel_editor_state_popup = new QListWidget(this);
    QLabel* temp = ui->channel_editor_state;
    channel_editor_state_popup->resize(94, 176);
    channel_editor_state_popup->move(temp->mapToGlobal(temp->rect().bottomLeft()) + QPoint(100, 5));
    channel_editor_state_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    channel_editor_state_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");
    channel_editor_state_popup->setVisible(false);

    channel_editor_state_popup_item[0] = new QListWidgetItem(QIcon(""), "Не задано");
    channel_editor_state_popup_item[1] = new QListWidgetItem(QIcon(""), "TETRA DMO");
    channel_editor_state_popup_item[2] = new QListWidgetItem(QIcon(""), "TETRA TMO");
    channel_editor_state_popup_item[3] = new QListWidgetItem(QIcon(""), "ВПД");
    channel_editor_state_popup_item[4] = new QListWidgetItem(QIcon(""), "АМ25");
    channel_editor_state_popup_item[5] = new QListWidgetItem(QIcon(""), "ЧМ25");
    channel_editor_state_popup_item[6] = new QListWidgetItem(QIcon(""), "ЧМ50");
    channel_editor_state_popup_item[7] = new QListWidgetItem(QIcon(""), "ОБП");
    channel_editor_state_popup_item[8] = new QListWidgetItem(QIcon(""), "FM радио");

    for (int i = 0; i < 9; i++) {
        channel_editor_state_popup->addItem(channel_editor_state_popup_item[i]);
    }

    connect(channel_editor_state_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_channel_editor_state_popup_itemSelectionChanged);
    channel_editor_state_popup->setCurrentItem(channel_editor_state_popup_item[0]);

    ui->ctcss->setProperty("chosen", 0);
    channel_editor_ctcss_popup = new QListWidget(this);
    channel_editor_ctcss_popup->resize(50, 220);
    channel_editor_ctcss_popup->move(ui->widget->mapToGlobal(ui->widget->rect().topRight()) + QPoint(45, 5)); // QPoint(-495, -5)
    channel_editor_ctcss_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    channel_editor_ctcss_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");
    channel_editor_ctcss_popup->setVisible(false);

{
    channel_editor_ctcss_popup_item[0] = new QListWidgetItem(QIcon(""), "Нет");
    channel_editor_ctcss_popup_item[1] = new QListWidgetItem(QIcon(""), "33.0");
    channel_editor_ctcss_popup_item[2] = new QListWidgetItem(QIcon(""), "35.4");
    channel_editor_ctcss_popup_item[3] = new QListWidgetItem(QIcon(""), "36.6");
    channel_editor_ctcss_popup_item[4] = new QListWidgetItem(QIcon(""), "37.9");
    channel_editor_ctcss_popup_item[5] = new QListWidgetItem(QIcon(""), "39.6");
    channel_editor_ctcss_popup_item[6] = new QListWidgetItem(QIcon(""), "44.4");
    channel_editor_ctcss_popup_item[7] = new QListWidgetItem(QIcon(""), "47.5");
    channel_editor_ctcss_popup_item[8] = new QListWidgetItem(QIcon(""), "49.2");
    channel_editor_ctcss_popup_item[9] = new QListWidgetItem(QIcon(""), "51.2");
    channel_editor_ctcss_popup_item[10] = new QListWidgetItem(QIcon(""), "53.0");
    channel_editor_ctcss_popup_item[11] = new QListWidgetItem(QIcon(""), "54.9");
    channel_editor_ctcss_popup_item[12] = new QListWidgetItem(QIcon(""), "56.8");
    channel_editor_ctcss_popup_item[13] = new QListWidgetItem(QIcon(""), "58.8");
    channel_editor_ctcss_popup_item[14] = new QListWidgetItem(QIcon(""), "63.0");
    channel_editor_ctcss_popup_item[15] = new QListWidgetItem(QIcon(""), "67.0");
    channel_editor_ctcss_popup_item[16] = new QListWidgetItem(QIcon(""), "69.4");
    channel_editor_ctcss_popup_item[17] = new QListWidgetItem(QIcon(""), "71.9");
    channel_editor_ctcss_popup_item[18] = new QListWidgetItem(QIcon(""), "74.4");
    channel_editor_ctcss_popup_item[19] = new QListWidgetItem(QIcon(""), "77.0");
    channel_editor_ctcss_popup_item[20] = new QListWidgetItem(QIcon(""), "79.7");
    channel_editor_ctcss_popup_item[21] = new QListWidgetItem(QIcon(""), "82.5");
    channel_editor_ctcss_popup_item[22] = new QListWidgetItem(QIcon(""), "85.4");
    channel_editor_ctcss_popup_item[23] = new QListWidgetItem(QIcon(""), "88.5");
    channel_editor_ctcss_popup_item[24] = new QListWidgetItem(QIcon(""), "91.5");
    channel_editor_ctcss_popup_item[25] = new QListWidgetItem(QIcon(""), "94.8");
    channel_editor_ctcss_popup_item[26] = new QListWidgetItem(QIcon(""), "97.4");
    channel_editor_ctcss_popup_item[27] = new QListWidgetItem(QIcon(""), "100.0");
    channel_editor_ctcss_popup_item[28] = new QListWidgetItem(QIcon(""), "103.5");
    channel_editor_ctcss_popup_item[29] = new QListWidgetItem(QIcon(""), "107.2");
    channel_editor_ctcss_popup_item[30] = new QListWidgetItem(QIcon(""), "110.9");
    channel_editor_ctcss_popup_item[31] = new QListWidgetItem(QIcon(""), "114.8");
    channel_editor_ctcss_popup_item[32] = new QListWidgetItem(QIcon(""), "118.8");
    channel_editor_ctcss_popup_item[33] = new QListWidgetItem(QIcon(""), "123.0");
    channel_editor_ctcss_popup_item[34] = new QListWidgetItem(QIcon(""), "127.3");
    channel_editor_ctcss_popup_item[35] = new QListWidgetItem(QIcon(""), "131.8");
    channel_editor_ctcss_popup_item[36] = new QListWidgetItem(QIcon(""), "136.5");
    channel_editor_ctcss_popup_item[37] = new QListWidgetItem(QIcon(""), "141.3");
    channel_editor_ctcss_popup_item[38] = new QListWidgetItem(QIcon(""), "146.2");
    channel_editor_ctcss_popup_item[39] = new QListWidgetItem(QIcon(""), "151.4");
    channel_editor_ctcss_popup_item[40] = new QListWidgetItem(QIcon(""), "156.7");
    channel_editor_ctcss_popup_item[41] = new QListWidgetItem(QIcon(""), "159.8");
    channel_editor_ctcss_popup_item[42] = new QListWidgetItem(QIcon(""), "162.2");
    channel_editor_ctcss_popup_item[43] = new QListWidgetItem(QIcon(""), "165.5");
    channel_editor_ctcss_popup_item[44] = new QListWidgetItem(QIcon(""), "167.9");
    channel_editor_ctcss_popup_item[45] = new QListWidgetItem(QIcon(""), "171.3");
    channel_editor_ctcss_popup_item[46] = new QListWidgetItem(QIcon(""), "173.8");
    channel_editor_ctcss_popup_item[47] = new QListWidgetItem(QIcon(""), "177.3");
    channel_editor_ctcss_popup_item[48] = new QListWidgetItem(QIcon(""), "179.9");
    channel_editor_ctcss_popup_item[49] = new QListWidgetItem(QIcon(""), "183.5");
    channel_editor_ctcss_popup_item[50] = new QListWidgetItem(QIcon(""), "186.2");
    channel_editor_ctcss_popup_item[51] = new QListWidgetItem(QIcon(""), "189.9");
    channel_editor_ctcss_popup_item[52] = new QListWidgetItem(QIcon(""), "192.8");
    channel_editor_ctcss_popup_item[53] = new QListWidgetItem(QIcon(""), "196.6");
    channel_editor_ctcss_popup_item[54] = new QListWidgetItem(QIcon(""), "199.5");
    channel_editor_ctcss_popup_item[55] = new QListWidgetItem(QIcon(""), "203.5");
    channel_editor_ctcss_popup_item[56] = new QListWidgetItem(QIcon(""), "206.5");
    channel_editor_ctcss_popup_item[57] = new QListWidgetItem(QIcon(""), "210.7");
    channel_editor_ctcss_popup_item[58] = new QListWidgetItem(QIcon(""), "218.1");
    channel_editor_ctcss_popup_item[59] = new QListWidgetItem(QIcon(""), "225.7");
    channel_editor_ctcss_popup_item[60] = new QListWidgetItem(QIcon(""), "229.1");
    channel_editor_ctcss_popup_item[61] = new QListWidgetItem(QIcon(""), "233.6");
    channel_editor_ctcss_popup_item[62] = new QListWidgetItem(QIcon(""), "241.8");
    channel_editor_ctcss_popup_item[63] = new QListWidgetItem(QIcon(""), "250.3");
    channel_editor_ctcss_popup_item[64] = new QListWidgetItem(QIcon(""), "254.1");
}
    for (int i = 0; i < 65; i++) {
        channel_editor_ctcss_popup->addItem(channel_editor_ctcss_popup_item[i]);
    }

    set_fonts();

    connect(channel_editor_ctcss_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_channel_editor_ctcss_popup_itemSelectionChanged);

    ui->scan->setProperty("chosen", 0);
    direction_editor_scan_popup = new QListWidget(this);
    direction_editor_scan_popup->resize(95, 125);
    direction_editor_scan_popup->move(ui->scan->mapToGlobal(ui->scan->rect().bottomLeft()) + QPoint(100, 55));
    direction_editor_scan_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    direction_editor_scan_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");
    direction_editor_scan_popup->setVisible(false);

    direction_editor_scan_popup->addItem("Нет");
    for(int i = 1; i <= 32; i++){
        direction_editor_scan_popup->addItem(QString::number(i));
    }

    connect(direction_editor_scan_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_direction_editor_scan_popup_itemSelectionChanged);

    broadcast_init();
}

void MainWindow::setup(){

}

MainWindow::~MainWindow()
{
    delete ui;
    udpSocket.leaveMulticastGroup(QHostAddress(ADDR));
    outpDev->close();
    delete inpt;
    delete outp;
    delete inptDev;
    delete outpDev;
}

void MainWindow::selfcontrol_screen()
{
    ui->mainPages->setCurrentWidget(ui->selfcontrol_page);


    ui->label_CPU->hide();
    ui->label_CPU_r->hide();
    ui->label_MPU->hide();
    ui->label_MPU_r->hide();
    ui->label_DSP->hide();
    ui->label_DSP_r->hide();
    ui->label_CRAM->hide();
    ui->label_CRAM_r->hide();
    ui->label_PRAM->hide();
    ui->label_PRAM_r->hide();
    ui->label_SDRAM->hide();
    ui->label_SDRAM_r->hide();
    ui->label_FLASH->hide();
    ui->label_FLASH_r->hide();
    ui->label_Klapan->hide();
    ui->label_Klapan_r->hide();
    ui->label_Dostup->hide();
    ui->label_Dostup_r->hide();
    ui->label_Zagruz4ik->hide();
    ui->label_Zagruz4ik_r->hide();
    ui->label_Program->hide();
    ui->label_Program_r->hide();

    ui->label_CPU->show();
    delay(20);
    ui->label_CPU_r->show();
    delay(20);
    ui->label_MPU->show();
    delay(20);
    ui->label_MPU_r->show();
    delay(20);
    ui->label_DSP->show();
    delay(20);
    ui->label_DSP_r->show();
    delay(20);
    ui->label_CRAM->show();
    delay(20);
    ui->label_CRAM_r->show();
    delay(20);
    ui->label_PRAM->show();
    delay(20);
    ui->label_PRAM_r->show();
    delay(20);
    ui->label_SDRAM->show();
    delay(4000);
    ui->label_SDRAM_r->show();
    delay(20);
    ui->label_FLASH->show();
    delay(20);
    ui->label_FLASH_r->show();
    delay(20);
    ui->label_Klapan->show();
    delay(20);
    ui->label_Klapan_r->show();
    delay(20);
    ui->label_Dostup->show();
    delay(20);
    ui->label_Dostup_r->show();
    delay(20);
    ui->label_Zagruz4ik->show();
    delay(20);
    ui->label_Zagruz4ik_r->show();
    delay(20);
    ui->label_Program->show();
    delay(3000);
    ui->label_Program_r->show();

    loading_screen();
}

void MainWindow::loading_screen()
{
    ui->mainPages->setCurrentWidget(ui->loading_page);
    ui->label_15->setStyleSheet("font-weight: bold;");
    ui->widget_8->setStyleSheet("font-weight: bold;");
}

void MainWindow::main_screen()
{
    ui->mainPages->setCurrentWidget(ui->main_page);

    if(current_direction != nullptr){
        ui->direction_label->setText(current_direction->name);
        if(current_direction->ch != nullptr){
            ui->channel_label->setText(current_direction->ch->name);
        }

        ui->main_background->setStyleSheet(
            "#main_background {"
            " border-image: url(:/resources/back (" + QString::number(current_direction->background) + ").png)"
            "}");

        ui->dejurnii_label->setText("Дежурный приём");
    }
    else{
        ui->main_background->setStyleSheet("background-color: black");
        ui->dejurnii_label->setText("");
    }
}

void MainWindow::menu_screen()
{
    ui->mainPages->setCurrentWidget(ui->menu_page);
    //WD
    //ui->menu_list->setCurrentItem(selected_items["menu_list"]);
}

void MainWindow::service_menu_screen()
{
    ui->mainPages->setCurrentWidget(ui->service_menu_page);
    //ui->service_menu_list->setCurrentItem(selected_items["service_menu_list"]);
}

void MainWindow::on_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == menu_list_item[5]){
        service_menu_screen();
    }
}

void MainWindow::on_menu_list_itemSelectionChanged()
{
    selected_items["menu_list"] = ui->menu_list->currentItem();
}

void MainWindow::on_service_menu_list_itemSelectionChanged()
{
    selected_items["service_menu_list"] = ui->service_menu_list->currentItem();
}

void MainWindow::on_data_editor_list_itemSelectionChanged()
{
    selected_items["data_editor_list"] = ui->data_editor_list->currentItem();
}

void MainWindow::on_channel_popup_menu_list_itemSelectionChanged()
{
    selected_items["channel_popup_menu_list"] = ui->channel_popup_menu_list->currentItem();
}

void MainWindow::on_direction_popup_menu_list_itemSelectionChanged()
{
    selected_items["direction_popup_menu_list"] = ui->direction_popup_menu_list->currentItem();
}

void MainWindow::on_direction_selection_list_itemSelectionChanged()
{
    selected_items["direction_selection_list"] = ui->direction_selection_list->currentItem();
}

void MainWindow::on_channel_list_itemSelectionChanged()
{
    selected_items["channel_list"] = ui->channel_list->currentItem();
}

void MainWindow::_on_channel_editor_state_popup_itemSelectionChanged()
{
    selected_items["channel_editor_state_popup"] = channel_editor_state_popup->currentItem();
}

void MainWindow::_on_channel_editor_ctcss_popup_itemSelectionChanged()
{
    selected_items["channel_editor_ctcss_popup"] = channel_editor_ctcss_popup->currentItem();
}
void MainWindow::_on_direction_editor_scan_popup_itemSelectionChanged()
{
    if(selected_items["direction_editor_scan_popup"]){
        selected_items["direction_editor_scan_popup"]->setBackground(QColor(255, 255, 255));
        selected_items["direction_editor_scan_popup"]->setTextColor(QColor(133, 165, 200));
    }

    selected_items["direction_editor_scan_popup"] = direction_editor_scan_popup->currentItem();

    selected_items["direction_editor_scan_popup"]->setBackground(QColor(56, 82, 130));
    selected_items["direction_editor_scan_popup"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_direction_list_itemSelectionChanged()
{
    if(selected_items["direction_list"]){
        selected_items["direction_list"]->setBackground(QColor(255, 255, 255));
        selected_items["direction_list"]->setTextColor(QColor(133, 165, 200));
    }

    selected_items["direction_list"] = ui->direction_list->currentItem();

    selected_items["direction_list"]->setBackground(QColor(56, 82, 130));
    selected_items["direction_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_channel_choice_list_itemSelectionChanged()
{
    if(selected_items["channel_choice_list"]){
        selected_items["channel_choice_list"]->setBackground(QColor(255, 255, 255));
        selected_items["channel_choice_list"]->setTextColor(QColor(133, 165, 200));
    }

    selected_items["channel_choice_list"] = ui->channel_choice_list->currentItem();

    selected_items["channel_choice_list"]->setBackground(QColor(56, 82, 130));
    selected_items["channel_choice_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_service_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == service_menu_list_item[7]){
        data_editor_screen();
    }
}

void MainWindow::data_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->data_editor_page);
    //WD
    //ui->data_editor_list->setCurrentItem(selected_items["data_editor_list"]);
}

void MainWindow::update_channel_list_screen()
{
    if(selected_items["channel_list"] == nullptr){
        ui->empty_channel_list_label->setVisible(true);
        //ui->empty_channel_list_label->setEnabled(true);
    }
    else{
        ui->empty_channel_list_label->setVisible(false);
        //ui->empty_channel_list_label->setEnabled(false);
    }
}

void MainWindow::update_direction_list_screen()
{
    //TODO
}

void MainWindow::channel_list_screen()
{
    ui->mainPages->setCurrentWidget(ui->channel_list_page);
    update_channel_list_screen();
    // WD
    //ui->channel_list->setCurrentItem(selected_items["channel_list"]);
}

void MainWindow::direction_list_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_list_page);
    update_direction_list_screen();
    //WD
    //ui->direction_list->setCurrentItem(selected_items["direction_list"]);
}

void MainWindow::on_data_editor_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == data_editor_list_item[1]){
        direction_list_screen();
    }
    if(item == data_editor_list_item[2]){
        channel_list_screen();
    }
}

void MainWindow::on_service_menu_right_clicked()
{
    menu_screen();
}

void MainWindow::on_service_menu_left_clicked()
{
    on_service_menu_list_itemDoubleClicked(selected_items["service_menu_list"]);
}

void MainWindow::on_channel_list_right_clicked()
{
    if(ui->channel_popup_menu->isVisible()){
        ui->channel_list_left->setText("Меню");
        ui->channel_popup_menu->setVisible(false);
        //ui->channel_popup_menu->setEnabled(false);
    }
    else{
        data_editor_screen();
    }
}

void MainWindow::on_direction_list_right_clicked()
{
    if(ui->direction_popup_menu->isVisible()){
        ui->direction_list_left->setText("Меню");
        ui->direction_popup_menu->setVisible(false);
        //ui->direction_popup_menu->setEnabled(false);
    }
    else{
        data_editor_screen();
    }
}

void MainWindow::on_data_editor_left_clicked()
{
    on_data_editor_list_itemDoubleClicked(selected_items["data_editor_list"]);
}

void MainWindow::on_data_editor_right_clicked()
{
    service_menu_screen();
}

void MainWindow::on_channel_list_left_clicked()
{
    if(ui->channel_popup_menu->isVisible()){
        // menu selection
        on_channel_popup_menu_list_itemDoubleClicked(selected_items["channel_popup_menu_list"]);
    }
    else{
        // menu activation
        //ui->channel_popup_menu->setEnabled(true);
        ui->channel_popup_menu->setVisible(true);
        ui->channel_popup_menu_list->setCurrentItem(selected_items["channel_popup_menu_list"]);
        ui->channel_list_left->setText("Выбрать");
    }
}

void MainWindow::on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    //look at the choice
    // EDIT
    if(item == channel_popup_menu_list_item[0]){
        if(selected_items["channel_list"] == nullptr) return;
        //else
        //ui->channel_popup_menu->setEnabled(false);
        ui->channel_popup_menu->setVisible(false);
        ui->channel_list_left->setText("Меню");
        channel_editor_screen();
    }
    // ADD
    if(item == channel_popup_menu_list_item[1]){
        QListWidgetItem* ref = new QListWidgetItem(QIcon(""), "");
        QListWidgetItem* ref2 = new QListWidgetItem(QIcon(""), "");
        Channel* new_ch = new Channel();
        channel_map[ref] = {new_ch, ref2};
        channel_map_d[ref2] = new_ch;

        // for channel
        ui->channel_list->addItem(ref);
        // for directions
        ui->channel_choice_list->addItem(ref2);

        // PA: when we work with QLists, we don't use selected_items[""] anymore, cause
        // use triggers on item selection changing. [check on_..._itemSelectionChanged() upper]
        // the triggers help with right selections

        //selected_items["channel_list"] = ref;
        ui->channel_list->setCurrentItem(ref);

        //ui->channel_popup_menu->setEnabled(false);
        ui->channel_popup_menu->setVisible(false);
        ui->channel_list_left->setText("Меню");
        channel_editor_screen();
    }
    // DELETE
    if(item == channel_popup_menu_list_item[2]){
        if(selected_items["channel_list"] != nullptr){

            // for channel
            ui->channel_list->removeItemWidget(selected_items["channel_list"]);
            // for directions
            ui->channel_choice_list->removeItemWidget(channel_map[selected_items["channel_list"]].ref2);
            channel_map_d.erase(channel_map[selected_items["channel_list"]].ref2);

            delete channel_map[selected_items["channel_list"]].channel;
            delete channel_map[selected_items["channel_list"]].ref2;
            channel_map.erase(selected_items["channel_list"]);

            // PA: we dont do it cause qt does it by itself and even better
            // selected_items["channel_list"] = nullptr;
            // delete a chosen item causes item Selection automatically from available items

            delete selected_items["channel_list"];

            // PA: a few lines above
            // selected_items["channel_list"] = channel_map.empty() ? nullptr : channel_map.begin()->first;
            // ui->channel_list->setCurrentItem(channel_map.empty() ? nullptr : channel_map.begin()->first);

            //ui->channel_popup_menu->setEnabled(false);
            ui->channel_popup_menu->setVisible(false);
            ui->channel_list_left->setText("Меню");
            update_channel_list_screen();
        }
    }
}

void MainWindow::set_default_channel_fields(){
    // dmo
    // tmo
    // vpd
    // am25

    // chm25
    // chm50 {the same}

    ui->is_forbidden_prd->setCheckState(Qt::Unchecked);
    ui->dualfreq->setCheckState(Qt::Unchecked);
    ui->channel_freq->setText("");
    ui->channel_prm_freq->setText("");
    ui->channel_prd_freq->setText("");
    channel_editor_ctcss_popup->setCurrentRow(0);
    ui->ctcss->setText(channel_editor_ctcss_popup->currentItem()->text());
    ui->channel_name->setText("");

    // obp
    // fm
}

// channel loading
void MainWindow::channel_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->channel_editor_page);

    Channel* curr = channel_map[selected_items["channel_list"]].channel;

    channel_editor_state_popup->setCurrentRow(curr->state);
    ui->channel_editor_state->setProperty("chosen", curr->state);
    ui->channel_editor_state->setText(channel_editor_state_popup->currentItem()->text());

    set_default_channel_fields();

    switch (curr->state) {
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:
    case 6:
        ui->is_forbidden_prd->setCheckState(curr->PRD ? Qt::Checked : Qt::Unchecked);
        ui->dualfreq->setCheckState(curr->dualfreq ? Qt::Checked : Qt::Unchecked);
        ui->channel_freq->setText(QString::number(curr->freq));
        ui->channel_prm_freq->setText(QString::number(curr->prm_freq));
        ui->channel_prd_freq->setText(QString::number(curr->prd_freq));
        ui->ctcss->setProperty("chosen", curr->ctcss);
        channel_editor_ctcss_popup->setCurrentRow(curr->ctcss);
        ui->ctcss->setText(channel_editor_ctcss_popup->currentItem()->text());
        ui->channel_name->setText(curr->name);
        break;
    case 7:

        break;
    case 8:

        break;
    }

    // Always point to the first field first
    curr_editor_field[channel_types[curr->state]] = 0;

    update_channel_editor_page();
}

void MainWindow::on_channel_editor_right_clicked()
{
    //TODO

    // types
    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        if(channel_editor_state_popup->isVisible()){
            channel_editor_state_popup->setVisible(false);
        }
        else{
            channel_editor_state_popup->setVisible(true);
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 0){
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 5){
        switch (curr_editor_field["chm25"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd->toggle();
            break;
        case 2:
            ui->dualfreq->toggle();
            break;
        case 3:
            ui->channel_freq->backspace();
            break;
        case 4:
            ui->channel_prm_freq->backspace();
            break;
        case 5:
            ui->channel_prd_freq->backspace();
            break;
        case 6:
            if(channel_editor_ctcss_popup->isVisible()){
                channel_editor_ctcss_popup->setVisible(false);
            }
            else{
                channel_editor_ctcss_popup->setVisible(true);
            }
            break;
        case 7:
            ui->channel_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 6){
        switch (curr_editor_field["chm50"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd->toggle();
            break;
        case 2:
            ui->dualfreq->toggle();
            break;
        case 3:
            ui->channel_freq->backspace();
            break;
        case 4:
            ui->channel_prm_freq->backspace();
            break;
        case 5:
            ui->channel_prd_freq->backspace();
            break;
        case 6:
            if(channel_editor_ctcss_popup->isVisible()){
                channel_editor_ctcss_popup->setVisible(false);
            }
            else{
                channel_editor_ctcss_popup->setVisible(true);
            }
            break;
        case 7:
            ui->channel_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

}

void MainWindow::on_channel_editor_left_clicked()
{
    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        if(channel_editor_state_popup->isVisible()){
            //Выбрать
            ui->channel_editor_state->setText(selected_items["channel_editor_state_popup"]->text());
            if(ui->channel_editor_state->property("chosen") != channel_editor_state_popup->currentRow()){
                channel_map[selected_items["channel_list"]].channel->clear();
                clear_chm25_fields();
                clear_chm50_fields();
                set_default_channel_fields();
            }
            ui->channel_editor_state->setProperty("chosen", channel_editor_state_popup->currentRow());
            channel_editor_state_popup->setVisible(false);
            update_channel_editor_page();
            return;
        }
    }

    if(ui->channel_editor_state->property("chosen") == 5){
        if(curr_editor_field["chm25"] == 6){
            if(channel_editor_ctcss_popup->isVisible()){
                //Выбрать
                ui->ctcss->setText(selected_items["channel_editor_ctcss_popup"]->text());
                ui->ctcss->setProperty("chosen", channel_editor_ctcss_popup->currentRow());
                channel_editor_ctcss_popup->setVisible(false);
                update_channel_editor_page();
                return;
            }
        }
    }

    if(ui->channel_editor_state->property("chosen") == 6){
        if(curr_editor_field["chm50"] == 6){
            if(channel_editor_ctcss_popup->isVisible()){
                //Выбрать
                ui->ctcss->setText(selected_items["channel_editor_ctcss_popup"]->text());
                ui->ctcss->setProperty("chosen", channel_editor_ctcss_popup->currentRow());
                channel_editor_ctcss_popup->setVisible(false);
                update_channel_editor_page();
                return;
            }
        }
    }

    // channel saving
    Channel* curr = channel_map[selected_items["channel_list"]].channel;

    // clearing before saving
    curr->clear();
    // REFACTOR
    curr->state = ui->channel_editor_state->property("chosen").toInt();

    switch (curr->state) {
    case 0:
        // skip
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        curr->PRD = ui->is_forbidden_prd->isChecked();
        curr->dualfreq = ui->dualfreq->isChecked();
        curr->freq = (uint32_t)ui->channel_freq->text().toInt();
        curr->prm_freq = (uint32_t)ui->channel_prm_freq->text().toInt();
        curr->prd_freq = (uint32_t)ui->channel_prd_freq->text().toInt();
        // REFACTOR
        curr->ctcss = ui->ctcss->property("chosen").toInt(); //channel_editor_ctcss_popup->currentRow();
        curr->name = ui->channel_name->text();
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
        return;
    }

    selected_items["channel_list"]->setText(curr->name);
    channel_map[selected_items["channel_list"]].ref2->setText(curr->name);
    channel_list_screen();
}

void MainWindow::on_channel_list_itemClicked(QListWidgetItem *item)
{
    //WDS we will disable this functionality soon. I mean cursor actions on the screen
    selected_items["channel_list"] = item;
}

void MainWindow::on_direction_list_left_clicked()
{
    if(ui->direction_popup_menu->isVisible()){
        // menu selection
        on_direction_popup_menu_list_itemDoubleClicked(selected_items["direction_popup_menu_list"]);
    }
    else{
        // menu activation
        //ui->direction_popup_menu->setEnabled(true);
        ui->direction_popup_menu->setVisible(true);
        ui->direction_popup_menu_list->setCurrentItem(selected_items["direction_popup_menu_list"]);
        ui->direction_list_left->setText("Выбрать");
    }
}

void MainWindow::on_direction_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    //look at the choice
    // EDIT
    if(item == direction_popup_menu_list_item[0]){
        if(selected_items["direction_list"] == nullptr) return;
        //ui->direction_popup_menu->setEnabled(false);
        ui->direction_popup_menu->setVisible(false);
        ui->direction_list_left->setText("Меню");
        direction_editor_screen();
    }
    // ADD
    if(item == direction_popup_menu_list_item[1]){
        QListWidgetItem* ref = new QListWidgetItem(QIcon(""), "");
        QListWidgetItem* ref2 = new QListWidgetItem(QIcon(""), "");
        Direction* new_dir = new Direction();
        direction_map[ref] = {new_dir, ref2};
        direction_map_d[ref2] = new_dir;

        ui->direction_list->addItem(ref);
        ui->direction_selection_list->addItem(ref2);

        // PA
        //selected_items["direction_list"] = ref;
        ui->direction_list->setCurrentItem(ref);

        //ui->direction_popup_menu->setEnabled(false);
        ui->direction_popup_menu->setVisible(false);
        ui->direction_list_left->setText("Меню");
        direction_editor_screen();
    }
    // DELETE
    if(item == direction_popup_menu_list_item[2]){
        if(selected_items["direction_list"] != nullptr){
            ui->direction_list->removeItemWidget(selected_items["direction_list"]);
            ui->direction_selection_list->removeItemWidget(direction_map[selected_items["direction_list"]].ref2);
            direction_map_d.erase(direction_map[selected_items["direction_list"]].ref2);

            if(current_direction == direction_map[selected_items["direction_list"]].direction){
                current_direction = nullptr;
            }
            delete direction_map[selected_items["direction_list"]].direction;
            delete direction_map[selected_items["direction_list"]].ref2;
            direction_map.erase(selected_items["direction_list"]);

            delete selected_items["direction_list"];

            // PA: the same in channel actions
            // selected_items["direction_list"] = direction_map.empty() ? nullptr : direction_map.begin()->first;
            // ui->direction_list->setCurrentItem(direction_map.empty() ? nullptr : direction_map.begin()->first);

            //ui->direction_popup_menu->setEnabled(false);
            ui->direction_popup_menu->setVisible(false);
            ui->direction_list_left->setText("Меню");
            //TODO ~update_direction_list
        }
    }
}

void MainWindow::set_default_direction_fields(){
    // dmo
    // tmo
    // vpd
    // am25
    // chm25

    ui->is_forbidden_prd_d->setCheckState(Qt::Unchecked);
    ui->is_tone_call->setCheckState(Qt::Unchecked);
    ui->scan->setProperty("chosen", 0);
    direction_editor_scan_popup->setCurrentRow(0);
    ui->scan->setText(direction_editor_scan_popup->currentItem()->text());
    ui->economizer->setProperty("num", 3);
    ui->economizer->setNum(3);
    _on_economizer_numChanged();
    ui->name_d->setText("");
    ui->background_dir_picture->setProperty("num", 0);
    ui->background_dir_picture->setNum(ui->background_dir_picture->property("num").toInt());
    _on_direction_background_numChanged();

    // chm50
    // obp
    // fm
}

// direction loading
void MainWindow::direction_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_editor_page);

    Direction* curr = direction_map[selected_items["direction_list"]].direction;

    set_default_direction_fields();

    if(curr->ch == nullptr){
        ui->channel_in_dir_name->setText("Не задано(Idle)");
        ui->channel_choice_list->setCurrentRow(0);
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        return;
    }

    ui->channel_in_dir_name->setText(curr->ch->name);

    if(curr->ch->state == 0){
        chosen_ref_d = 0; // ?
        swap_direction_page();
        update_direction_editor_page();
        return;
    }

    for(const auto& p : channel_map_d){
        if(p.second == curr->ch){
            ui->channel_choice_list->setCurrentItem(p.first);
            chosen_ref_d = p.first;
            break;
        }
    }
    swap_direction_page();
    update_direction_editor_page();


    switch (curr->ch->state) {
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    case 3:

        break;
    case 4:

        break;
    case 5:
        ui->is_forbidden_prd_d->setCheckState(curr->PRD ? Qt::Checked : Qt::Unchecked);
        ui->is_tone_call->setCheckState(curr->tone_call ? Qt::Checked : Qt::Unchecked);
        ui->scan->setProperty("chosen", curr->scan_list);
        direction_editor_scan_popup->setCurrentRow(curr->scan_list);
        ui->scan->setText(direction_editor_scan_popup->currentItem()->text());
        ui->economizer->setProperty("num", curr->economizer);
        ui->economizer->setNum(curr->economizer);
        _on_economizer_numChanged();
        ui->name_d->setText(curr->name);
        ui->background_dir_picture->setProperty("num", curr->background);
        ui->background_dir_picture->setNum(ui->background_dir_picture->property("num").toInt());
        _on_direction_background_numChanged();
        break;
    case 6:

        break;
    case 7:

        break;
    case 8:

        break;
    }

    // Always point to the first field first
    curr_editor_field[direction_types[curr->ch->state]] = 0;
}

void MainWindow::on_channel_choice_list_itemDoubleClicked(QListWidgetItem *item)
{
    direction_map[selected_items["direction_list"]].direction->ch = channel_map_d[item];
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
    ui->channel_in_dir_name->setText(channel_map_d[item]->name);
}

void MainWindow::_on_economizer_numChanged()
{
    switch (ui->economizer->property("num").toInt()) {
    case 0:
        ui->label_34->setText("0ms / 0ms");
        break;
    case 1:
        ui->label_34->setText("50ms / 500ms");
        break;
    case 2:
        ui->label_34->setText("100ms / 1000ms");
        break;
    case 3:
        ui->label_34->setText("200ms / 2000ms");
        break;
    default:
        break;
    }
}

void MainWindow::_on_direction_background_numChanged()
{
    ui->label_37->setStyleSheet("border-image: url(:/resources/back (" + QString::number(ui->background_dir_picture->property("num").toInt()) + ").png);");
}

// direction saving
void MainWindow::on_direction_editor_left_clicked()
{
    if(chosen_ref_d == 0 || curr_editor_field[direction_types[channel_map_d[chosen_ref_d]->state]] == 0){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            if(selected_items["channel_choice_list"] == 0){
                return;
            }
            //Выбрать
            chosen_ref_d = selected_items["channel_choice_list"];
            ui->channel_in_dir_name->setText(channel_map_d[chosen_ref_d]->name);
            swap_direction_page();
            update_direction_editor_page();
            return;
        }
    }

    // a try to save without a channel
    if(chosen_ref_d == 0){
        Direction* curr = direction_map[selected_items["direction_list"]].direction;
        curr->ch = new Channel();
        curr->ch->state = 0;
        curr->ch->name = "Канал не определен";
        curr->name = "Idle";
        selected_items["direction_list"]->setText(curr->name + "\n ");
        selected_items["direction_list"]->setIcon(QIcon(":/resources/picture32.png"));
        direction_map[selected_items["direction_list"]].ref2->setText(curr->name);
        direction_map[selected_items["direction_list"]].ref2->setIcon(QIcon(":/resources/picture32.png"));
        direction_list_screen();
        return;
    }

    Channel* channel = channel_map_d[chosen_ref_d];
    if(channel->state == 5){
        if(curr_editor_field["chm25_d"] == 3){
            if(direction_editor_scan_popup->isVisible()){
                //Выбрать
                ui->scan->setText(selected_items["direction_editor_scan_popup"]->text());
                ui->scan->setProperty("chosen", direction_editor_scan_popup->currentRow());
                direction_editor_scan_popup->setVisible(false);
                update_direction_editor_page();
                return;
            }
        }
    }

    // direction saving
    Direction* curr = direction_map[selected_items["direction_list"]].direction;
    // clearing before saving
    curr->clear();
    // REFACTOR
    curr->ch = channel; //channel_map_d[selected_items["channel_choice_list"]];

    if(curr->ch){
        switch (curr->ch->state) {
        case 0:
            // skip
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            curr->PRD = ui->is_forbidden_prd_d->isChecked();
            curr->tone_call = ui->is_tone_call->isChecked();
            curr->scan_list = ui->scan->property("chosen").toInt();
            curr->economizer = ui->economizer->property("num").toInt();
            curr->name = ui->name_d->text();
            curr->background = ui->background_dir_picture->property("num").toInt();
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        }

        selected_items["direction_list"]->setText(curr->name + "\n" + curr->ch->name);
        selected_items["direction_list"]->setIcon(QIcon(":/resources/picture32.png"));
        direction_map[selected_items["direction_list"]].ref2->setText(curr->name); // + "\n" + curr->ch->name
        direction_map[selected_items["direction_list"]].ref2->setIcon(QIcon(":/resources/picture32.png"));
    }
    direction_list_screen();
}

void MainWindow::on_direction_editor_right_clicked()
{
    // TODO
    // need to erase Выбрать when we have loaded an Idle direction

    if(chosen_ref_d == 0 || curr_editor_field[direction_types[channel_map_d[chosen_ref_d]->state]] == 0){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            //Назад
            if(chosen_ref_d) ui->channel_choice_list->setCurrentItem(chosen_ref_d);
            swap_direction_page();
            update_direction_editor_page();
            return;
        }
        else{
            //Выбрать
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);
            update_direction_editor_page();
            return;
        }
    }

    if(channel_map_d[chosen_ref_d]->state == 5){
        switch (curr_editor_field["chm25_d"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 3:
            if(direction_editor_scan_popup->isVisible()){
                direction_editor_scan_popup->setVisible(false);
            }
            else{
                direction_editor_scan_popup->setVisible(true);
            }
            break;
        case 4:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 5:
            ui->name_d->backspace();
            break;
        case 6:
            ui->background_dir_picture->setProperty("num", (ui->background_dir_picture->property("num").toInt() + 1) % 10);
            ui->background_dir_picture->setNum(ui->background_dir_picture->property("num").toInt());
            _on_direction_background_numChanged();
            break;
        default:
            qCritical("crit: on_direction_editor_right_clicked");
            return;
        }
        update_direction_editor_page();
        return;
    }

}

void MainWindow::on_channel_in_dir_name_clicked()
{
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);
}

void MainWindow::on_channel_choice_list_itemClicked(QListWidgetItem *item)
{
    // WDS
    selected_items["channel_choice_list"] = item;
}

void MainWindow::on_direction_list_itemClicked(QListWidgetItem *item)
{
    // WDS
    selected_items["direction_list"] = item;
}

void MainWindow::on_main_left_clicked()
{
    menu_screen();
}

void MainWindow::on_main_right_clicked()
{
    direction_selection_screen();
}

void MainWindow::on_menu_left_clicked()
{
    //TODO?
    ui->menu_list->itemDoubleClicked(ui->menu_list->currentItem());
}

void MainWindow::on_menu_right_clicked()
{
    main_screen();
}

void MainWindow::on_direction_selection_left_clicked()
{
    if(selected_items["direction_selection_list"] != nullptr){
        current_direction = direction_map_d[selected_items["direction_selection_list"]];
    }
    else{
        current_direction = nullptr;
    }
    main_screen();
}

void MainWindow::on_direction_selection_right_clicked()
{
    main_screen();
}

void MainWindow::direction_selection_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_selection_page);
    if(selected_items["direction_selection_list"] == nullptr){
        if(direction_map_d.empty()){
            // PA
            //selected_items["direction_selection_list"] = nullptr;
            ui->direction_selection_list->setCurrentItem(nullptr);
        }
        else{
            // PA
            //selected_items["direction_selection_list"] = direction_map_d.begin()->first;
            ui->direction_selection_list->setCurrentItem(direction_map_d.begin()->first);
        }
    }
}

void MainWindow::on_direction_selection_list_itemClicked(QListWidgetItem *item)
{
    //WDS
    selected_items["direction_selection_list"] = item;
}

void MainWindow::broadcast_init()
{
    udpSocket.bind(QHostAddress::AnyIPv4, PORT, QUdpSocket::ShareAddress);
    udpSocket.joinMulticastGroup(QHostAddress(ADDR));
    connect(&udpSocket, &QUdpSocket::readyRead,
        this, &MainWindow::recieveDatagrams);

    QAudioFormat format_in;
    format_in.setSampleRate(8000);
    format_in.setChannelCount(1);
    format_in.setSampleSize(8);
    format_in.setCodec("audio/pcm");
    format_in.setByteOrder(QAudioFormat::LittleEndian);
    format_in.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format_in))
        format_in = info.nearestFormat(format_in);

    QAudioFormat format_out;
    format_out.setSampleRate(8000);
    format_out.setChannelCount(1);
    format_out.setSampleSize(8);
    format_out.setCodec("audio/pcm");
    format_out.setByteOrder(QAudioFormat::LittleEndian);
    format_out.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info2(QAudioDeviceInfo::defaultOutputDevice());
    if (!info2.isFormatSupported(format_out))
        format_out = info2.nearestFormat(format_out);

    inpt = new QAudioInput(format_in, this);
    outp = new QAudioOutput(format_out, this);
    inpt->setBufferSize(BUF_SZ);
    outp->setBufferSize(BUF_SZ);

    outpDev = outp->start();

    connect(&udpSocket, SIGNAL(readyRead()), this, SLOT(recieveDatagrams()), Qt::QueuedConnection);
}

void MainWindow::recieveDatagrams()
{
    QByteArray datagram;
    while (udpSocket.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket.pendingDatagramSize()));
        udpSocket.readDatagram(datagram.data(), datagram.size());
    }
    int freq = getFreq();
    int incoming_freq;
    from_byte_array(datagram.constData(), incoming_freq);
    if (incoming_freq == freq && !transmitting)
    {
        buffer.append(datagram.constData() + 4, datagram.size() - 4);
        playSamples();
    }
}

void MainWindow::playSamples()
{
    outpDev->write(buffer);
    buffer.clear();
}

void MainWindow::sendDatagrams()
{
    QByteArray datagram;
    int freq = getFreq();
    to_byte_array(freq_bytes, freq);
    datagram.append(freq_bytes, 4);
    datagram.append(inptDev->readAll());
    udpSocket.writeDatagram(datagram.data(), datagram.size(),
        QHostAddress(ADDR), PORT);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(current_direction != nullptr && current_direction->ch != nullptr)
    {
        if(event->key() == Qt::Key_1 && !transmitting)
        {
            transmitting = true;
            inptDev = inpt->start();
            inptConn = connect(inptDev, &QIODevice::readyRead,
                this, &MainWindow::sendDatagrams, Qt::QueuedConnection);
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(current_direction != nullptr && current_direction->ch != nullptr)
    {
        if(event->key() == Qt::Key_1 && !event->isAutoRepeat())
        {
            transmitting = false;
            inptDev->close();
            inpt->stop();
            disconnect(inptConn);
        }
    }
}

inline int MainWindow::getFreq(){
    if(current_direction == nullptr || current_direction->ch == nullptr) return 0;
    return (int)current_direction->ch->freq;
}
// WD where is it from??? mb main_right_clicked() ?
/*
void MainWindow::on_direction_button_clicked()
{
    direction_selection_screen();
}
*/
/*
                     /////
                    /////=========
                    \\\\\=========
                     \\\\\
*/
void MainWindow::on_left_arrow_clicked()
{
    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->loading_page){
        if(is_open_communication){
            main_screen();
        }
        return;
    }
    if(curr == ui->main_page){
        ui->main_left->click();
        return;
    }
    if(curr == ui->menu_page){
        ui->menu_left->click();
        return;
    }
    if(curr == ui->service_menu_page){
        ui->service_menu_left->click();
        return;
    }
    if(curr == ui->data_editor_page){
        ui->data_editor_left->click();
        return;
    }
    if(curr == ui->channel_list_page){
        ui->channel_list_left->click();
        return;
    }
    if(curr == ui->direction_list_page){
        ui->direction_list_left->click();
        return;
    }
    if(curr == ui->direction_selection_page){
        ui->direction_selection_left->click();
        return;
    }
    if(curr == ui->channel_editor_page){
        ui->channel_editor_left->click();
        return;
    }
    if(curr == ui->direction_editor_page){
        ui->direction_editor_left->click();
        return;
    }
}

/*
                   \\\\\
            -=-=-=-=\\\\\
            -=-=-=-=/////
                   /////
*/
void MainWindow::on_right_arrow_clicked()
{
    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->main_page){
        ui->main_right->click();
        return;
    }
    if(curr == ui->menu_page){
        ui->menu_right->click();
        return;
    }
    if(curr == ui->service_menu_page){
        ui->service_menu_right->click();
        return;
    }
    if(curr == ui->data_editor_page){
        ui->data_editor_right->click();
        return;
    }
    if(curr == ui->channel_list_page){
        ui->channel_list_right->click();
        return;
    }
    if(curr == ui->direction_list_page){
        ui->direction_list_right->click();
        return;
    }
    if(curr == ui->direction_selection_page){
        ui->direction_selection_right->click();
        return;
    }
    if(curr == ui->channel_editor_page){
        ui->channel_editor_right->click();
        return;
    }
    if(curr == ui->direction_editor_page){
        ui->direction_editor_right->click();
        return;
    }
}

void MainWindow::on_number_1_clicked()
{
    on_number_i_clicked(1);
}
void MainWindow::on_number_2_clicked()
{
    on_number_i_clicked(2);
}
void MainWindow::on_number_3_clicked()
{
    on_number_i_clicked(3);
}
void MainWindow::on_number_4_clicked()
{
    on_number_i_clicked(4);
}
void MainWindow::on_number_5_clicked()
{
    on_number_i_clicked(5);
}
void MainWindow::on_number_6_clicked()
{
    on_number_i_clicked(6);
}
void MainWindow::on_number_7_clicked()
{
    on_number_i_clicked(7);
}
void MainWindow::on_number_8_clicked()
{
    on_number_i_clicked(8);
}
void MainWindow::on_number_9_clicked()
{
    on_number_i_clicked(9);
}
void MainWindow::on_number_0_clicked()
{
    on_number_i_clicked(0);
}

void MainWindow::on_number_i_clicked(int i)
{
    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->channel_editor_page){
        //chm25
        if(ui->channel_editor_state->property("chosen") == 5){
            switch (curr_editor_field["chm25"]) {
            case 3:
                ui->channel_freq->setText(ui->channel_freq->text() + QString::number(i));
                break;
            case 4:
                ui->channel_prm_freq->setText(ui->channel_prm_freq->text() + QString::number(i));
                break;
            case 5:
                ui->channel_prd_freq->setText(ui->channel_prd_freq->text() + QString::number(i));
                break;
            case 7:
                /* letters!
                ' ' 1
                А Б В 2
                Г Д Е Ё 3
                Ж З И Й 4
                К Л М Н 5
                О П Р С 6
                Т У Ф Х 7
                Ц Ч Ш 8
                Щ Ъ Ы 9
                Ь Э Ю Я 0
                */
                // TODO

                break;
            default:
                ;
            }
            return;
        }

        if(ui->channel_editor_state->property("chosen") == 6){
            switch (curr_editor_field["chm50"]) {
            case 3:
                ui->channel_freq->setText(ui->channel_freq->text() + QString::number(i));
                break;
            case 4:
                ui->channel_prm_freq->setText(ui->channel_prm_freq->text() + QString::number(i));
                break;
            case 5:
                ui->channel_prd_freq->setText(ui->channel_prd_freq->text() + QString::number(i));
                break;
            case 7:
                /* letters!
                ' ' 1
                А Б В 2
                Г Д Е Ё 3
                Ж З И Й 4
                К Л М Н 5
                О П Р С 6
                Т У Ф Х 7
                Ц Ч Ш 8
                Щ Ъ Ы 9
                Ь Э Ю Я 0
                */
                // TODO

                break;
            default:
                ;
            }
            return;
        }
    }
}

void go_up(QListWidget* qlw, uint size){
    qlw->setCurrentRow((qlw->currentRow() - 1 + size) % size);
}

void MainWindow::clear_chm25_fields(){
    ui->channel_editor_state->setStyleSheet("");
    ui->is_forbidden_prd->setStyleSheet("");
    ui->dualfreq->setStyleSheet("");
    ui->channel_freq_full->setStyleSheet("");
    ui->channel_prm_freq_full->setStyleSheet("");
    ui->channel_prd_freq_full->setStyleSheet("");
    ui->ctcss->setStyleSheet("");
    ui->channel_name->setStyleSheet("");
    ui->label_32->setVisible(false);
    ui->label_42->setVisible(false);
    ui->label_39->setVisible(false);
}

void MainWindow::clear_chm25_d_fields(){
    ui->channel_in_dir_name->setStyleSheet("text-align: left;");

    ui->is_forbidden_prd_d->setStyleSheet("");
    ui->is_tone_call->setStyleSheet("");
    ui->scan->setStyleSheet("");
    ui->economizer->setStyleSheet("");

    ui->name_d->setStyleSheet("");
    ui->background_dir_picture->setStyleSheet("");
}

void MainWindow::clear_chm50_fields(){
    ui->channel_editor_state->setStyleSheet("");
    ui->is_forbidden_prd->setStyleSheet("");
    ui->dualfreq->setStyleSheet("");
    ui->channel_freq_full->setStyleSheet("");
    ui->channel_prm_freq_full->setStyleSheet("");
    ui->channel_prd_freq_full->setStyleSheet("");
    ui->ctcss->setStyleSheet("");
    ui->channel_name->setStyleSheet("");
    ui->label_32->setVisible(false);
    ui->label_42->setVisible(false);
    ui->label_39->setVisible(false);
}

void MainWindow::update_channel_editor_page(){
    //it will be hard...

    // swap page if necessary
    switch (ui->channel_editor_state->property("chosen").toInt()) {
    case 0:
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 1:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 2:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 3:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 4:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 5:
        if(ui->channel_editor_states->currentWidget() != ui->CHM25_page)
            ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
        break;
    case 6:
        if(ui->channel_editor_states->currentWidget() != ui->CHM25_page)
            ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
        break;
    case 7:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    case 8:
        // TODO: CHANGE
        if(ui->channel_editor_states->currentWidget() != ui->empty_state_page)
            ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
        break;
    default:
        qCritical("crit: update_channel_editor_page: channel_editor_state");
    }

    // change buttons at type changing
    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        clear_chm25_fields();
        // clear_chm50_fields();
        ui->channel_editor_state->setStyleSheet("border: 1px solid blue;");
        if(channel_editor_state_popup->isVisible()){
            ui->channel_editor_left->setText("Выбрать");
            ui->channel_editor_right->setText("Назад");
        }
        else{
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Выбрать");
        }
        //return;
    }

    // change buttons for chosen type
    // none
    if(ui->channel_editor_state->property("chosen") == 0){
        // was upper
        return;
    }

    // chm25
    if(ui->channel_editor_state->property("chosen") == 5){
        clear_chm25_fields();
        switch (curr_editor_field["chm25"]) {
        case 0:
            // was upper
            ui->channel_editor_state->setStyleSheet("border: 1px solid blue;");
            break;
        case 1:
            ui->is_forbidden_prd->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->dualfreq->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->channel_freq_full->setStyleSheet("#channel_freq_full {border: 1px solid blue;}");
            ui->label_32->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->channel_prm_freq_full->setStyleSheet("#channel_prm_freq_full {border: 1px solid blue;}");
            ui->label_42->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->channel_prd_freq_full->setStyleSheet("#channel_prd_freq_full {border: 1px solid blue;}");
            ui->label_39->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->ctcss->setStyleSheet("border: 1px solid blue;");
            if(channel_editor_ctcss_popup->isVisible()){
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            else{
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->channel_name->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        default:
            qCritical("chm25: update_channel_editor_page: no way");
        }

        if(ui->dualfreq->isChecked()){
            ui->widget_6->setVisible(true);
            //ui->widget_6->setEnabled(true);
            ui->widget_9->setVisible(true);
            //ui->widget_9->setEnabled(true);
            ui->widget_4->setVisible(false);
            //ui->widget_4->setEnabled(false);
        }
        else{
            ui->widget_6->setVisible(false);
            //ui->widget_6->setEnabled(false);
            ui->widget_9->setVisible(false);
            //ui->widget_9->setEnabled(false);
            ui->widget_4->setVisible(true);
            //ui->widget_4->setEnabled(true);
        }
        return;
    }

    // chm50
    if(ui->channel_editor_state->property("chosen") == 6){
        clear_chm50_fields();
        switch (curr_editor_field["chm50"]) {
        case 0:
            // was upper
            ui->channel_editor_state->setStyleSheet("border: 1px solid blue;");
            break;
        case 1:
            ui->is_forbidden_prd->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->dualfreq->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->channel_freq_full->setStyleSheet("#channel_freq_full {border: 1px solid blue;}");
            ui->label_32->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->channel_prm_freq_full->setStyleSheet("#channel_prm_freq_full {border: 1px solid blue;}");
            ui->label_42->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->channel_prd_freq_full->setStyleSheet("#channel_prd_freq_full {border: 1px solid blue;}");
            ui->label_39->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->ctcss->setStyleSheet("border: 1px solid blue;");
            if(channel_editor_ctcss_popup->isVisible()){
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            else{
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->channel_name->setStyleSheet("border: 1px solid blue;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        default:
            qCritical("chm50: update_channel_editor_page: no way");
        }

        if(ui->dualfreq->isChecked()){
            ui->widget_6->setVisible(true);
            //ui->widget_6->setEnabled(true);
            ui->widget_9->setVisible(true);
            //ui->widget_9->setEnabled(true);
            ui->widget_4->setVisible(false);
            //ui->widget_4->setEnabled(false);
        }
        else{
            ui->widget_6->setVisible(false);
            //ui->widget_6->setEnabled(false);
            ui->widget_9->setVisible(false);
            //ui->widget_9->setEnabled(false);
            ui->widget_4->setVisible(true);
            //ui->widget_4->setEnabled(true);
        }
        return;
    }
}

void MainWindow::swap_direction_page(){
    if(chosen_ref_d == 0){
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        return;
    }

    switch (channel_map_d[chosen_ref_d]->state) {
    case 0:
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 1:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 2:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 3:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 4:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 5:
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->direction_tuner_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
        break;
    case 6:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 7:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    case 8:
        // TODO: CHANGE
        if(ui->direction_editor_stackedWidget->currentWidget() != ui->empty_direction_editor_page)
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
        break;
    default:
        qCritical("crit: direction_editor_stackedWidget");
    }
}

void MainWindow::update_direction_editor_page(){
    clear_chm25_d_fields();

    if(chosen_ref_d == nullptr || curr_editor_field[direction_types[channel_map_d[chosen_ref_d]->state]] == 0){
        if(chosen_ref_d){
            ui->channel_in_dir_name->setStyleSheet("border: 1px solid blue; text-align: left;");
        }
        else{
            ui->channel_in_dir_name->setStyleSheet("text-align: left;");
        }
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){            
            if(channel_map_d.empty()){
                ui->direction_editor_left->setText("");
            }
            else{
                ui->direction_editor_left->setText("Выбрать");
            }
            ui->direction_editor_right->setText("Назад");
        }
        else{
            ui->direction_editor_left->setText("Сохранить");
            // TODO
            // need to erase Вставить when we have loaded an Idle direction
            ui->direction_editor_right->setText("Выбрать");

        }
        return;
    }

    Channel* channel = channel_map_d[chosen_ref_d];

    // change buttons for chosen type
    // none
    if(channel->state == 0){
        // was upper
        return;
    }

    // chm25_d
    if(channel->state == 5){
        clear_chm25_d_fields();
        switch (curr_editor_field["chm25_d"]) {
        case 0:
            // was upper
            //ui->channel_in_dir_name->setStyleSheet("border: 1px solid blue; text-align: left;");
            break;
        case 1:
            ui->is_forbidden_prd_d->setStyleSheet("border: 1px solid blue;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 2:
            ui->is_tone_call->setStyleSheet("border: 1px solid blue;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 3:
            ui->scan->setStyleSheet("border: 1px solid blue;");
            if(direction_editor_scan_popup->isVisible()){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 4:
            ui->economizer->setStyleSheet("border: 1px solid blue;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 5:
            ui->name_d->setStyleSheet("border: 1px solid blue;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            break;
        case 6:
            ui->background_dir_picture->setStyleSheet("border: 1px solid blue;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("chm25_d: update_direction_editor_page: no way");
        }
        return;
    }
}

/*

              //\\
             ///\\\
            ////\\\\
               ==
               ==
               ==
               ==
*/

void MainWindow::on_up_arrow_clicked()
{
    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->loading_page){
        //changing type
        if(!is_open_communication){
            is_open_communication = true;
            ui->label_14->setStyleSheet("background: rgb(0, 0, 255);");
            ui->widget_7->setStyleSheet("background: rgb(0, 0, 255);");

            ui->label_15->setStyleSheet("font-weight: bold;");
            ui->widget_8->setStyleSheet("font-weight: bold;");
        }
        return;
    }
    if(curr == ui->menu_page){
        go_up(ui->menu_list, menu_list_size);
        return;
    }
    if(curr == ui->service_menu_page){
        go_up(ui->service_menu_list, service_menu_list_size);
        return;
    }
    if(curr == ui->data_editor_page){
        go_up(ui->data_editor_list, data_editor_list_size);
        return;
    }
    if(curr == ui->channel_list_page){
        if(ui->channel_popup_menu->isVisible()){
            //change selection in menu list
            go_up(ui->channel_popup_menu_list, 3);
        }
        else{
            //change selection in channel list
            if(!channel_map.empty()){
                go_up(ui->channel_list, channel_map.size());
            }
        }
        return;
    }
    if(curr == ui->direction_list_page){
        if(ui->direction_popup_menu->isVisible()){
            //change selection in menu list
            go_up(ui->direction_popup_menu_list, 3);
        }
        else{
            //change selection in direction list
            if(!direction_map.empty()){
                go_up(ui->direction_list, direction_map.size());
            }
        }
        return;
    }
    if(curr == ui->direction_selection_page){
        if(!direction_map.empty()){
            go_up(ui->direction_selection_list, direction_map.size());
        }
        return;
    }
    if(curr == ui->channel_editor_page){

        if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
            if(channel_editor_state_popup->isVisible()){
                go_up(channel_editor_state_popup, 9);
                return;
            }
        }

        // none
        if(ui->channel_editor_state->property("chosen") == 0){
            return;
        }

        // chm25
        if(ui->channel_editor_state->property("chosen") == 5){
            if(curr_editor_field["chm25"] == 6){
                if(channel_editor_ctcss_popup->isVisible()){
                    go_up(channel_editor_ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm25"].size();
            curr_editor_field["chm25"] = (curr_editor_field["chm25"] - 1 + sz) % sz;
            if(ui->dualfreq->isChecked()){
                if(curr_editor_field["chm25"] == 3) curr_editor_field["chm25"]--;
            }
            else{
                if(curr_editor_field["chm25"] == 5) curr_editor_field["chm25"] -= 2;
            }
            update_channel_editor_page();
            return;
        }

        // chm50
        if(ui->channel_editor_state->property("chosen") == 6){
            if(curr_editor_field["chm50"] == 6){
                if(channel_editor_ctcss_popup->isVisible()){
                    go_up(channel_editor_ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm50"].size();
            curr_editor_field["chm50"] = (curr_editor_field["chm50"] - 1 + sz) % sz;
            if(ui->dualfreq->isChecked()){
                if(curr_editor_field["chm50"] == 3) curr_editor_field["chm50"]--;
            }
            else{
                if(curr_editor_field["chm50"] == 5) curr_editor_field["chm50"] -= 2;
            }
            update_channel_editor_page();
            return;
        }
    }
    if(curr == ui->direction_editor_page){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->empty_direction_editor_page){
            return;
        }

        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            if(!channel_map_d.empty()) go_up(ui->channel_choice_list, channel_map_d.size());
            return;
        }

        Channel* channel = channel_map_d[chosen_ref_d];

        // none
        if(channel->state == 0){
            return;
        }

        if(channel->state == 5){

            if(curr_editor_field["chm25_d"] == 3){
                if(direction_editor_scan_popup->isVisible()){
                    go_up(direction_editor_scan_popup, 33);
                    return;
                }
            }

            uint sz = editor_fields["chm25_d"].size();
            curr_editor_field["chm25_d"] = (curr_editor_field["chm25_d"] - 1 + sz) % sz;

            update_direction_editor_page();
            return;
        }
    }
}

void go_down(QListWidget* qlw, uint size){
    qlw->setCurrentRow((qlw->currentRow() + 1) % size);
}

/*

               ==
               ==
               ==
               ==
            \\\\////
             \\\///
              \\//

*/

void MainWindow::on_down_arrow_clicked()
{
    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->loading_page){
        //changing type
        if(is_open_communication){
            is_open_communication = false;
            ui->label_14->setStyleSheet("font-weight: bold;");
            ui->widget_7->setStyleSheet("font-weight: bold; ");

            ui->label_15->setStyleSheet("background: rgb(0, 0, 255);");
            ui->widget_8->setStyleSheet("background: rgb(0, 0, 255);");
        }
        return;
    }
    if(curr == ui->menu_page){
        go_down(ui->menu_list, menu_list_size);
        return;
    }
    if(curr == ui->service_menu_page){
        go_down(ui->service_menu_list, service_menu_list_size);
        return;
    }
    if(curr == ui->data_editor_page){
        go_down(ui->data_editor_list, data_editor_list_size);
        return;
    }
    if(curr == ui->channel_list_page){
        if(ui->channel_popup_menu->isVisible()){
            //change selection in menu list
            go_down(ui->channel_popup_menu_list, 3);
        }
        else{
            //change selection in channel list
            if(!channel_map.empty()){
                go_down(ui->channel_list, channel_map.size());
            }
        }
        return;
    }
    if(curr == ui->direction_list_page){
        if(ui->direction_popup_menu->isVisible()){
            //change selection in menu list
            go_down(ui->direction_popup_menu_list, 3);
        }
        else{
            //change selection in direction list
            if(!direction_map.empty()){
                go_down(ui->direction_list, direction_map.size());
            }
        }
        return;
    }
    if(curr == ui->direction_selection_page){
        if(!direction_map.empty()){
            go_down(ui->direction_selection_list, direction_map.size());
        }
        return;
    }
    if(curr == ui->channel_editor_page){
        if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
            if(channel_editor_state_popup->isVisible()){
                go_down(channel_editor_state_popup, 9);
                return;
            }
        }

        // none
        if(ui->channel_editor_state->property("chosen") == 0){
            return;
        }

        // chm25
        // REFACTOR
        if(ui->channel_editor_state->property("chosen") == 5){

            if(curr_editor_field["chm25"] == 6){
                if(channel_editor_ctcss_popup->isVisible()){
                    go_down(channel_editor_ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm25"].size();
            curr_editor_field["chm25"] = (curr_editor_field["chm25"] + 1) % sz;
            if(ui->dualfreq->isChecked()){
                if(curr_editor_field["chm25"] == 3) curr_editor_field["chm25"]++;
            }
            else{
                if(curr_editor_field["chm25"] == 4) curr_editor_field["chm25"] += 2;
            }

            update_channel_editor_page();
            return;
        }

        // chm50
        if(ui->channel_editor_state->property("chosen") == 6){

            if(curr_editor_field["chm50"] == 6){
                if(channel_editor_ctcss_popup->isVisible()){
                    go_down(channel_editor_ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm50"].size();
            curr_editor_field["chm50"] = (curr_editor_field["chm50"] + 1) % sz;
            if(ui->dualfreq->isChecked()){
                if(curr_editor_field["chm50"] == 3) curr_editor_field["chm50"]++;
            }
            else{
                if(curr_editor_field["chm50"] == 4) curr_editor_field["chm50"] += 2;
            }

            update_channel_editor_page();
            return;
        }
    }
    if(curr == ui->direction_editor_page){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->empty_direction_editor_page){
            return;
        }

        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            if(!channel_map_d.empty()) go_down(ui->channel_choice_list, channel_map_d.size());
            return;
        }

        Channel* channel = channel_map_d[chosen_ref_d];

        // none
        if(channel->state == 0){
            return;
        }

        if(channel->state == 5){

            if(curr_editor_field["chm25_d"] == 3){
                if(direction_editor_scan_popup->isVisible()){
                    go_down(direction_editor_scan_popup, 33);
                    return;
                }
            }

            uint sz = editor_fields["chm25_d"].size();
            curr_editor_field["chm25_d"] = (curr_editor_field["chm25_d"] + 1) % sz;

            update_direction_editor_page();
            return;
        }
    }
}

void MainWindow::on_left_tube_clicked()
{
    auto curr = ui->mainPages->currentWidget();
}

void MainWindow::on_right_tube_clicked()
{
    auto curr = ui->mainPages->currentWidget();
}

void MainWindow::on_dualfreq_clicked()
{
    if(ui->dualfreq->isChecked()){
        ui->widget_6->setVisible(true);
        //ui->widget_6->setEnabled(true);
        ui->widget_9->setVisible(true);
        //ui->widget_9->setEnabled(true);
        ui->widget_4->setVisible(false);
        //ui->widget_4->setEnabled(false);
    }
    else{
        ui->widget_6->setVisible(false);
        //ui->widget_6->setEnabled(false);
        ui->widget_9->setVisible(false);
        //ui->widget_9->setEnabled(false);
        ui->widget_4->setVisible(true);
        //ui->widget_4->setEnabled(true);
    }
}
