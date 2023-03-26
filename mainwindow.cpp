#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QtMultimedia>
#include <iostream>
#include <QScrollBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QGraphicsDropShadowEffect>
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

    auto _sz = MainWindow::size();
    setMinimumSize(_sz);
    setMaximumSize(_sz);

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
    //                              0       1      2      3      4       5       6       7
    editor_fields["tmo"] = { "type", "net", "mcc", "mnc", "gssi", "vesh", "mask", "name" };
    curr_editor_field["tmo"] = 0;
    //                        0       1      2      3       4       5
    editor_fields["vpd"] = { "type", "mcc", "mnc", "gssi", "freq", "name" };
    curr_editor_field["vpd"] = 0;
    //                          0       1      2        3       4           5          6
    editor_fields["am25"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "name" };
    curr_editor_field["am25"] = 0;
    //                          0       1      2        3       4           5           6        7
    editor_fields["chm25"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "ctcss", "name" };
    curr_editor_field["chm25"] = 0;
    //                          0       1      2        3       4           5           6        7
    editor_fields["chm50"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "ctcss", "name" };
    curr_editor_field["chm50"] = 0;
    //                          0     1      2       3       4
    editor_fields["obp"] = { "type", "prd", "band", "freq", "name" };
    curr_editor_field["obp"] = 0;
    //                          0     1      2       3       4
    editor_fields["fm"] = { "type", "prd", "2freq", "freq", "prm_freq", "prd_freq", "name" };
    curr_editor_field["fm"] = 0;

    //                         0
    editor_fields["none_d"] = { "type" };
    curr_editor_field["none_d"] = 0;
    //                            0          1      2       3       4             5       6
    editor_fields["chm25_d"] = { "channel", "prd", "tone", "scan", "economizer", "name", "background" };
    curr_editor_field["chm25_d"] = 0;
    curr_editor_field["chm50_d"] = 0;

    //channel_editor_state
    ui->ch_type_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->ch_type_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");

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
        ui->ch_type_popup->addItem(channel_editor_state_popup_item[i]);
    }

    connect(ui->ch_type_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_channel_editor_state_popup_itemSelectionChanged);
    ui->ch_type_popup->setCurrentItem(channel_editor_state_popup_item[0]);

    ui->chm25_ctcss->setProperty("chosen", 0);

    ui->ctcss_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    ui->ctcss_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");


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
        ui->ctcss_popup->addItem(channel_editor_ctcss_popup_item[i]);
    }

    set_styles();

    connect(ui->ctcss_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_channel_editor_ctcss_popup_itemSelectionChanged);

    ui->scan->setProperty("chosen", 0);
    direction_editor_scan_popup = new QListWidget(this);
    direction_editor_scan_popup->resize(95, 120);
    direction_editor_scan_popup->move(ui->scan->mapToGlobal(ui->scan->rect().bottomLeft()) + QPoint(100, 100));
    direction_editor_scan_popup->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    direction_editor_scan_popup->verticalScrollBar()->setStyleSheet("QScrollBar {width:3px;}");
    direction_editor_scan_popup->setVisible(false);

    direction_editor_scan_popup->addItem("Нет");
    for(int i = 1; i <= 32; i++){
        direction_editor_scan_popup->addItem(QString::number(i));
    }

    connect(direction_editor_scan_popup, &QListWidget::itemSelectionChanged, this, &MainWindow::_on_direction_editor_scan_popup_itemSelectionChanged);

    show3d = new QAction("3D", this);
    connect(show3d, &QAction::triggered, this, &MainWindow::show_3d);
    ui->menuBar->addAction(show3d);

    showrd = new QAction("Радиоданные", this);
    connect(showrd, &QAction::triggered, this, &MainWindow::show_radiodata);
    ui->menuBar->addAction(showrd);

    shownorm = new QAction("Норматив", this);
    connect(shownorm, &QAction::triggered, this, &MainWindow::show_normative);
    ui->menuBar->addAction(shownorm);

    broadcast_init();

    //ui->dej->setVisible(false);
    ui->modals->setCurrentWidget(ui->no_modals);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(1);
    effect->setXOffset(2);
    effect->setYOffset(2);
    effect->setColor(Qt::black);
    ui->widget_18->setGraphicsEffect(effect);
    ui->widget_29->setGraphicsEffect(effect);
    ui->widget_30->setGraphicsEffect(effect);

    ui->modals->move(250, 210);
    ui->atuners->move(250, 210);
}

void MainWindow::setup(){

}

void MainWindow::show_3d()
{
    _3dwin.show();
}

void MainWindow::show_radiodata()
{
    rdwin.show();
}

void MainWindow::show_normative()
{
    normwin.show();
}

MainWindow::~MainWindow()
{
    delete ui;
    udpSocket.leaveMulticastGroup(QHostAddress(ADDR));
    outpDev->close();
}

void MainWindow::off_screen(){
    ui->mainPages->setCurrentWidget(ui->offscreen);
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
    ui->modals->setCurrentWidget(ui->no_modals);
    //ui->dej->setVisible(false);
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
    if(item == menu_list_item[0]){
        main_screen();
        ui->atuners->setCurrentWidget(ui->noise);
        ui->noise->setProperty("from_menu", true);
        noise_show();
        return;
    }
    if(item == menu_list_item[1]){
        main_screen();
        ui->atuners->setCurrentWidget(ui->volume);
        ui->volume->setProperty("from_menu", true);
        volume_show();
        return;
    }
    if(item == menu_list_item[5]){
        service_menu_screen();
        return;
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
    selected_items["channel_editor_state_popup"] = ui->ch_type_popup->currentItem();
}

void MainWindow::_on_channel_editor_ctcss_popup_itemSelectionChanged()
{
    selected_items["channel_editor_ctcss_popup"] = ui->ctcss_popup->currentItem();
}
void MainWindow::_on_direction_editor_scan_popup_itemSelectionChanged()
{

    selected_items["direction_editor_scan_popup"] = direction_editor_scan_popup->currentItem();
}

void MainWindow::on_direction_list_itemSelectionChanged()
{
    selected_items["direction_list"] = ui->direction_list->currentItem();
}

void MainWindow::on_channel_choice_list_itemSelectionChanged()
{
    selected_items["channel_choice_list"] = ui->channel_choice_list->currentItem();
}

bool check_password(const QString &pw){
    return true;
}

void MainWindow::on_service_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == service_menu_list_item[0]){
        RS485_PRM_screen();
    }
    if(item == service_menu_list_item[1]){
        RS485_PRD_screen();
    }
    if(item == service_menu_list_item[2]){
        USB_PRM_screen();
    }
    if(item == service_menu_list_item[3]){
        BL_PRM_screen();
    }
    if(item == service_menu_list_item[7]){
        ui->label_16->setText("");
        ui->password->setProperty("password", "");
        ui->modals->setCurrentWidget(ui->password);
        ui->service_menu_left->setText("Далее");
        ui->service_menu_right->setText("Стереть");
    }
}

void MainWindow::data_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->data_editor_page);
    //WD
    //ui->data_editor_list->setCurrentItem(selected_items["data_editor_list"]);
}

void MainWindow::RS485_PRM_screen(){
    ui->mainPages->setCurrentWidget(ui->RS485_PRM_page);
}

void MainWindow::RS485_PRD_screen(){
    ui->mainPages->setCurrentWidget(ui->RS485_PRD_page);
    ui->modals->setCurrentWidget(ui->privilege_level);
}

void MainWindow::USB_PRM_screen(){
    ui->mainPages->setCurrentWidget(ui->USB_PRM_page);
}

void MainWindow::BL_PRM_screen(){
    ui->mainPages->setCurrentWidget(ui->BL_PRM_page);
    ui->modals->setCurrentWidget(ui->privilege_level);
}

void MainWindow::BL_PRD_screen(){
    ui->mainPages->setCurrentWidget(ui->BL_PRD_page);
    ui->modals->setCurrentWidget(ui->privilege_level);
}

void MainWindow::IR_PRM_screen(){
    ui->mainPages->setCurrentWidget(ui->IR_PRM_page);
}

void MainWindow::IR_PRD_screen(){
    ui->mainPages->setCurrentWidget(ui->IR_PRD_page);
}

void MainWindow::update_channel_list_screen()
{
    if(selected_items["channel_list"] == nullptr){
        ui->empty_channel_list_label->setVisible(true);
    }
    else{
        ui->empty_channel_list_label->setVisible(false);
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
    if(ui->modals->currentWidget() == ui->password){
        ui->label_16->setText(ui->label_16->text().remove(0, 1));
        auto temp = ui->password->property("password").toString();
        temp.resize(temp.length() - 1);
        ui->password->setProperty("password", temp);
        return;
    }
    menu_screen();
}

void MainWindow::on_service_menu_left_clicked()
{
    if(ui->modals->currentWidget() == ui->password){
        if(check_password(ui->password->property("password").toString())){
            ui->modals->setCurrentWidget(ui->no_modals);
            ui->service_menu_left->setText("Выбрать");
            ui->service_menu_right->setText("Назад");
            data_editor_screen();
            return;
        }
    }
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

bool MainWindow::delete_channel(QListWidgetItem *item){
    if(item == nullptr) return false;

    // for channel
    ui->channel_list->removeItemWidget(item);
    // for directions
    ui->channel_choice_list->removeItemWidget(channel_map[item].ref2);
    channel_map_d.erase(channel_map[item].ref2);

    delete channel_map[item].channel;
    delete channel_map[item].ref2;
    channel_map.erase(item);

    // PA: we dont do it cause qt does it by itself and even better
    // selected_items["channel_list"] = nullptr;
    // delete a chosen item causes item Selection automatically from available items

    delete item;

    // PA: a few lines above
    // selected_items["channel_list"] = channel_map.empty() ? nullptr : channel_map.begin()->first;
    // ui->channel_list->setCurrentItem(channel_map.empty() ? nullptr : channel_map.begin()->first);

    return true;
}

void MainWindow::on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    //look at the choice
    // EDIT
    if(item == channel_popup_menu_list_item[0]){
        if(selected_items["channel_list"] == nullptr) return;
        //else
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

        ui->channel_popup_menu->setVisible(false);
        ui->channel_list_left->setText("Меню");
        channel_editor_screen();
    }
    // DELETE
    if(item == channel_popup_menu_list_item[2]){
        if(delete_channel(selected_items["channel_list"])){
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

    ui->chm25_prd->setCheckState(Qt::Unchecked);
    ui->chm25_dualfreq->setCheckState(Qt::Unchecked);
    ui->chm25_freq->setText("");
    ui->chm25_prm_freq->setText("");
    ui->chm25_prd_freq->setText("");
    ui->ctcss_popup->setCurrentRow(0);
    ui->chm25_ctcss->setText(ui->ctcss_popup->currentItem()->text());
    ui->chm25_name->setText("");

    // obp
    // fm
}

// channel loading
void MainWindow::channel_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->channel_editor_page);

    Channel* curr = channel_map[selected_items["channel_list"]].channel;

    ui->ch_type_popup->setCurrentRow(curr->state);
    ui->channel_editor_state->setProperty("chosen", curr->state);
    ui->channel_editor_state->setText(ui->ch_type_popup->currentItem()->text());

    set_default_channel_fields();

    switch (curr->state) {
    case 0: // none
        break;
    case 1: // dmo
    {
        break;
    }
    case 2: // tmo
    {

        break;
    }
    case 3: // vpd
    {
        ui->vpd_mcc->setText(curr->mcc);
        ui->vpd_mnc->setText(curr->mnc);
        ui->vpd_gssi->setText(curr->gssi);
        ui->vpd_freq->setText(QString::number(curr->freq));
        ui->vpd_name->setText(curr->name);
        break;
    }
    case 4: // am25
    {
        ui->am25_prd->setChecked(curr->PRD);
        ui->am25_dualfreq->setChecked(curr->dualfreq);
        ui->am25_freq->setText(QString::number(curr->freq));
        ui->am25_prm_freq->setText(QString::number(curr->prm_freq));
        ui->am25_prd_freq->setText(QString::number(curr->prd_freq));
        ui->am25_name->setText(curr->name);
        break;
    }
    case 5: // chm25
    {
        ui->chm25_prd->setChecked(curr->PRD);
        ui->chm25_dualfreq->setChecked(curr->dualfreq);
        ui->chm25_freq->setText(QString::number(curr->freq));
        ui->chm25_prm_freq->setText(QString::number(curr->prm_freq));
        ui->chm25_prd_freq->setText(QString::number(curr->prd_freq));
        ui->chm25_ctcss->setProperty("chosen", curr->ctcss);
        ui->ctcss_popup->setCurrentRow(curr->ctcss);
        ui->chm25_ctcss->setText(ui->ctcss_popup->currentItem()->text());
        ui->chm25_name->setText(curr->name);
        break;
    }
    case 6: // chm50
    {
        ui->chm50_prd->setChecked(curr->PRD);
        ui->chm50_dualfreq->setChecked(curr->dualfreq);
        ui->chm50_freq->setText(QString::number(curr->freq));
        ui->chm50_prm_freq->setText(QString::number(curr->prm_freq));
        ui->chm50_prd_freq->setText(QString::number(curr->prd_freq));
        ui->chm50_ctcss->setProperty("chosen", curr->ctcss);
        ui->ctcss_popup->setCurrentRow(curr->ctcss);
        ui->chm50_ctcss->setText(ui->ctcss_popup->currentItem()->text());
        ui->chm50_name->setText(curr->name);
        break;
    }
    case 7: // obp
    {
        ui->obp_prd->setChecked(curr->PRD);
        ui->obp_band->setProperty("band", curr->band);
        ui->obp_band->setText(curr->band ? "Верхняя" : "Нижняя");
        ui->obp_freq->setText(QString::number(curr->freq));
        ui->obp_name->setText(curr->name);
        break;
    }
    case 8: // fm

        break;
    }

    // Always point to the first field first
    curr_editor_field[channel_types[curr->state]] = 0;

    update_channel_editor_page();
}

void MainWindow::on_channel_editor_right_clicked()
{
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

    // types
    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        if(ui->modals->currentWidget() == ui->channel_type){
            ui->modals->setCurrentWidget(ui->no_modals);
        }
        else{
            ui->modals->setCurrentWidget(ui->channel_type);
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 0){
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 2){
        switch (curr_editor_field["tmo"]) {
        case 0:
            // skip
            break;
        case 1:
            // skip
            break;
        case 2:
            ui->tmo_mcc->backspace();
            break;
        case 3:
            ui->tmo_mnc->backspace();
            break;
        case 4:
            ui->tmo_gssi->backspace();
            break;
        case 5:
            ui->tmo_vesh->toggle();
            break;
        case 6:
            ui->tmo_mask->toggle();
            break;
        case 7:
            ui->tmo_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 3){
        switch (curr_editor_field["vpd"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->vpd_mcc->backspace();
            break;
        case 2:
            ui->vpd_mnc->backspace();
            break;
        case 3:
            ui->vpd_gssi->backspace();
            break;
        case 4:
            ui->vpd_freq->backspace();
            break;
        case 5:
            ui->vpd_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 4){
        switch (curr_editor_field["am25"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->am25_prd->toggle();
            break;
        case 2:
            ui->am25_dualfreq->toggle();
            break;
        case 3:
            ui->am25_freq->backspace();
            break;
        case 4:
            ui->am25_prm_freq->backspace();
            break;
        case 5:
            ui->am25_prd_freq->backspace();
            break;
        case 6:
            ui->am25_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 5){
        switch (curr_editor_field["chm25"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->chm25_prd->toggle();
            break;
        case 2:
            ui->chm25_dualfreq->toggle();
            break;
        case 3:
            ui->chm25_freq->backspace();
            break;
        case 4:
            ui->chm25_prm_freq->backspace();
            break;
        case 5:
            ui->chm25_prd_freq->backspace();
            break;
        case 6:
            if(ui->modals->currentWidget() == ui->ctcss){
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
                ui->modals->setCurrentWidget(ui->ctcss);
            }
            break;
        case 7:
            ui->chm25_name->backspace();
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
            ui->chm50_prd->toggle();
            break;
        case 2:
            ui->chm50_dualfreq->toggle();
            break;
        case 3:
            ui->chm50_freq->backspace();
            break;
        case 4:
            ui->chm50_prm_freq->backspace();
            break;
        case 5:
            ui->chm50_prd_freq->backspace();
            break;
        case 6:
            if(ui->modals->currentWidget() == ui->ctcss){
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
                ui->modals->setCurrentWidget(ui->ctcss);
            }
            break;
        case 7:
            ui->chm50_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 7){
        switch (curr_editor_field["obp"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->obp_prd->toggle();
            break;
        case 2:
            ui->obp_band->setProperty("band", ui->obp_band->property("band") == 1 ? 0 : 1); // 1 - верхняя, 0 - нижняя
            ui->obp_band->setText(ui->obp_band->property("band") == 1 ? "Верхняя" : "Нижняя");
            break;
        case 3:
            ui->obp_freq->backspace();
            break;
        case 4:
            ui->obp_name->backspace();
            break;
        case 5:
            ui->obp_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }

    if(ui->channel_editor_state->property("chosen") == 8){
        switch (curr_editor_field["fm"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->fm_prd->toggle();
            break;
        case 2:
            ui->fm_dualfreq->toggle();
            break;
        case 3:
            ui->fm_freq->backspace();
            break;
        case 4:
            ui->fm_prm_freq->backspace();
            break;
        case 5:
            ui->fm_prd_freq->backspace();
            break;
        case 6:
            ui->fm_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
        return;
    }
}

bool in_range(uint32_t num, uint32_t left, uint32_t right){
    return num >= left && num < right;
}

void MainWindow::on_channel_editor_left_clicked()
{
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        if(ui->modals->currentWidget() == ui->channel_type){
            //Выбрать
            ui->channel_editor_state->setText(selected_items["channel_editor_state_popup"]->text());
            if(ui->channel_editor_state->property("chosen") != ui->ch_type_popup->currentRow()){
                channel_map[selected_items["channel_list"]].channel->clear();
                clear_chm25_fields();
                clear_chm50_fields();
                set_default_channel_fields();
            }
            ui->channel_editor_state->setProperty("chosen", ui->ch_type_popup->currentRow());
            ui->modals->setCurrentWidget(ui->no_modals);
            update_channel_editor_page();
            return;
        }
    }

    if(ui->channel_editor_state->property("chosen") == 4){
        //nothing
    }

    if(ui->channel_editor_state->property("chosen") == 5){
        if(curr_editor_field["chm25"] == 6){
            if(ui->modals->currentWidget() == ui->ctcss){
                //Выбрать
                ui->chm25_ctcss->setText(selected_items["channel_editor_ctcss_popup"]->text());
                ui->chm25_ctcss->setProperty("chosen", ui->ctcss_popup->currentRow());
                ui->modals->setCurrentWidget(ui->no_modals);
                update_channel_editor_page();
                return;
            }
        }
    }

    if(ui->channel_editor_state->property("chosen") == 6){
        if(curr_editor_field["chm50"] == 6){
            if(ui->modals->currentWidget() == ui->ctcss){
                //Выбрать
                ui->chm50_ctcss->setText(selected_items["channel_editor_ctcss_popup"]->text());
                ui->chm50_ctcss->setProperty("chosen", ui->ctcss_popup->currentRow());
                ui->modals->setCurrentWidget(ui->no_modals);
                update_channel_editor_page();
                return;
            }
        }
    }

    // channel saving
    Channel* curr = channel_map[selected_items["channel_list"]].channel;
    curr->is_new = false;

#define ERR ui->modals->setCurrentWidget(ui->params_error); return; \

    uint32_t left, right;

    int state = ui->channel_editor_state->property("chosen").toInt();
    using u32 = uint32_t;
    // check save option
    switch (state)
    {
    case 0: // none
        // skip
        break;
    case 1: // dmo not implemented
        break;
    case 2: // tmo not implemented
        break;
    case 3: // vpd
    {
        left = 1000000;
        right = 1000000000;
        u32 mcc = ui->vpd_mcc->text().toInt();
        u32 mnc = ui->vpd_mnc->text().toInt();
        u32 gssi = ui->vpd_gssi->text().toInt();
        if (!in_range(mcc, 0, 999) || !in_range(mnc, 0, 999) || !in_range(gssi, 0, 16777215))
        {
            ERR
        }
        u32 freq = ui->vpd_freq->text().toInt();
        if (!in_range(freq, left, right))
        {
            ERR
        }
        break;
    }
    case 4: // am25
    {
        left = 1000000;
        right = 1000000000;
        if (ui->am25_dualfreq->isChecked())
        {
            u32 am25_prm_freq = ui->am25_prm_freq->text().toInt();
            u32 am25_prd_freq = ui->am25_prd_freq->text().toInt();
            if (!in_range(am25_prm_freq, left, right) || !in_range(am25_prd_freq, left, right))
            {
                ERR
            }
        }
        else
        {
            u32 freq = ui->am25_freq->text().toInt();
            if (!in_range(freq, left, right))
            {
                ERR
            }
        }
        if (ui->am25_name->text().isEmpty())
        {
            ERR
        }
        break;
    }
    case 5: // chm25
    {
        left = 1000000;
        right = 1000000000;
        if (ui->chm25_dualfreq->isChecked())
        {
            u32 chm25_prm_freq = ui->chm25_prm_freq->text().toInt();
            u32 chm25_prd_freq = ui->chm25_prd_freq->text().toInt();
            if (!in_range(chm25_prm_freq, left, right) || !in_range(chm25_prd_freq, left, right))
            {
                ERR
            }
        }
        else
        {
            u32 freq = ui->chm25_freq->text().toInt();
            if (!in_range(freq, left, right))
            {
                ERR
            }
        }

        if (ui->chm25_name->text().isEmpty())
        {
            ERR
        }
        break;
    }
    case 6: // chm50
    {
        left = 1000000;
        right = 1000000000;
        if (ui->chm50_dualfreq->isChecked())
        {
            u32 chm50_prm_freq = ui->chm50_prm_freq->text().toInt();
            u32 chm50_prd_freq = ui->chm50_prd_freq->text().toInt();
            if (!in_range(chm50_prm_freq, left, right) || !in_range(chm50_prd_freq, left, right))
            {
                ERR
            }
        }
        else
        {
            u32 freq = ui->chm50_freq->text().toInt();
            if (!in_range(freq, left, right))
            {
                ERR
            }
        }

        if (ui->chm25_name->text().isEmpty())
        {
            ERR
        }
        break;
    }
    case 7: // obp
    {
        left = 1000000;
        right = 1000000000;
        u32 freq = ui->obp_freq->text().toInt();
        if (!in_range(freq, left, right))
        {
            ERR
        }

        if (ui->obp_name->text().isEmpty())
        {
            ERR
        }
        break;
    }
    case 8: // fm
    {
        left = 1000000;
        right = 1000000000;
        if (ui->fm_dualfreq->isChecked())
        {
            u32 fm_prm_freq = ui->fm_prm_freq->text().toInt();
            u32 fm_prd_freq = ui->fm_prd_freq->text().toInt();
            if (!in_range(fm_prm_freq, left, right) || !in_range(fm_prd_freq, left, right))
            {
                ERR
            }
        }
        else
        {
            u32 freq = ui->fm_freq->text().toInt();
            if (!in_range(freq, left, right))
            {
                ERR
            }
        }
        if (ui->fm_name->text().isEmpty())
        {
            ERR
        }
        break;
    }
    }

    // clearing before saving
    curr->clear();

    curr->state = state;

    switch (curr->state)
    {
    case 0: // none
        // skip
        break;
    case 1: // dmo not implemented
        break;
    case 2: // tmo not implemented
        break;
    case 3:  // vpd
    {
        curr->mcc = ui->vpd_mcc->text().trimmed();
        curr->mnc = ui->vpd_mnc->text().trimmed();
        curr->gssi = ui->vpd_gssi->text().trimmed();
        curr->freq = ui->vpd_freq->text().toInt();
        curr->name = ui->vpd_name->text();
        break;
    }
    case 4: // am25
    {
        curr->PRD = ui->am25_prd->isChecked();
        curr->dualfreq = ui->am25_dualfreq->isChecked();
        curr->freq = ui->am25_freq->text().toInt();
        curr->prm_freq = ui->am25_prm_freq->text().toInt();
        curr->prd_freq = ui->am25_prd_freq->text().toInt();
        curr->name = ui->am25_name->text();
        break;
    }
    case 5: // chm25
    {
        curr->PRD = ui->chm25_prd->isChecked();
        curr->dualfreq = ui->chm25_dualfreq->isChecked();
        curr->freq = ui->chm25_freq->text().toInt();
        curr->prm_freq = ui->chm25_prm_freq->text().toInt();
        curr->prd_freq = ui->chm25_prd_freq->text().toInt();
        curr->ctcss = ui->chm25_ctcss->property("chosen").toInt();
        curr->name = ui->chm25_name->text();
        break;
    }
    case 6: // chm50
    {
        curr->PRD = ui->chm50_prd->isChecked();
        curr->dualfreq = ui->chm50_dualfreq->isChecked();
        curr->freq = ui->chm50_freq->text().toInt();
        curr->prm_freq = ui->chm50_prm_freq->text().toInt();
        curr->prd_freq = ui->chm50_prd_freq->text().toInt();
        curr->ctcss = ui->chm50_ctcss->property("chosen").toInt();
        curr->name = ui->chm50_name->text();
        break;
    }
    case 7: // obp
    {
        curr->PRD = ui->obp_prd->isChecked();
        curr->band = ui->obp_band->property("band") == 1 ? 0 : 1;
        curr->freq = ui->obp_freq->text().toInt();
        curr->name = ui->obp_name->text();
        break;
    }
    case 8: // fm
    {
        curr->PRD = ui->fm_prd->isChecked();
        curr->dualfreq = ui->fm_dualfreq->isChecked();
        curr->freq = ui->fm_freq->text().toInt();
        curr->prm_freq = ui->fm_prm_freq->text().toInt();
        curr->prd_freq = ui->fm_prd_freq->text().toInt();
        curr->name = ui->fm_name->text();
        break;
    }
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

// TODO: this scheisse
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
    if(ui->noise->property("from_menu").toBool() == true){
        noise_show();
        // and mb something yet
        return;
    }
    if(ui->volume->property("from_menu").toBool() == true){
        volume_handler(ui->volume->property("clicked_times").toInt());
        return;
    }
    menu_screen();
}

void MainWindow::on_main_right_clicked()
{
    if(ui->noise->property("from_menu").toBool() == true){
        noise_handler(ui->noise->property("clicked_times").toInt());
        return;
    }
    if(ui->volume->property("from_menu").toBool() == true){
        ui->volume->setProperty("from_menu", false);
        volume_handler(ui->volume->property("clicked_times").toInt());
    }
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
        broadcast_init();
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
    ui->modals->setCurrentWidget(ui->no_modals);
    //ui->dej->setVisible(false);
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

void MainWindow::hideDej(){
    if(!--receivedPackets){
        ui->modals->setCurrentWidget(ui->no_modals);
        //ui->dej->setVisible(false);
    }
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
        setReceiving();
        if(ui->mainPages->currentWidget() == ui->main_page){
            ui->modals->setCurrentWidget(ui->pr_per);
            //ui->dej->setVisible(true);
        }
        QTimer timer;
        timer.start(1000);
        receivedPackets++;
        connect(&timer, &QTimer::timeout, this, &MainWindow::hideDej);
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

void MainWindow::setReceiving(){
    ui->arrow->setStyleSheet("border-image: url(:/resources/pr.png)");

}

void MainWindow::setTransmitting(){
    ui->arrow->setStyleSheet("border-image: url(:/resources/per.png)");
    char buf[300] = "";
    Channel *ch = direction_map_d[selected_items["direction_selection_list"]]->ch;
    //ui->dej_label_1->setText(tr("ПРИЕМ 59.000"));
    //ui->dej_label_2->setText(tr("ЧМ25"));
    sprintf(buf, "ПЕРЕДАЧА %d.%03d", (ch->dualfreq ? ch->prd_freq / 1000000 : ch->freq / 1000000), (ch->dualfreq ? (ch->prd_freq / 1000) % 1000 : (ch->freq / 1000) % 1000));
    ui->dej_label_1->setText(tr(buf));
    switch (ch->state) {
    case 0:
        ui->dej_label_2->setText(tr("Idle"));
        break;
    case 1:
        ui->dej_label_2->setText(tr("TETRA_DMO"));
        break;
    case 2:
        ui->dej_label_2->setText(tr("tmo"));
        break;
    case 3:
        ui->dej_label_2->setText(tr("ВПД"));
        break;
    case 4:
        ui->dej_label_2->setText(tr("АМ25"));
        break;
    case 5:
        ui->dej_label_2->setText(tr("ЧМ25"));
        break;
    case 6:
        ui->dej_label_2->setText(tr("ЧМ50"));
        break;
    case 7:
        ui->dej_label_2->setText(tr("ОБП"));
        break;
    case 8:
        ui->dej_label_2->setText(tr("FM радио"));
        break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(current_direction != nullptr && current_direction->ch != nullptr)
    {
        if(event->key() == Qt::Key_1 && !transmitting)
        {
            transmitting = true;

            setTransmitting();
            if(ui->mainPages->currentWidget() == ui->main_page){
                ui->modals->setCurrentWidget(ui->pr_per);
                //ui->dej->setVisible(true);
            }

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

            ui->modals->setCurrentWidget(ui->no_modals);
            //ui->dej->setVisible(false);

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
    if(curr == ui->RS485_PRM_page){
        ui->RS485_PRM_right->click();
        return;
    }
    if(curr == ui->RS485_PRD_page){
        ui->RS485_PRD_right->click();
        return;
    }
    if(curr == ui->USB_PRM_page){
        ui->RS485_PRM_right->click();
        return;
    }
    if(curr == ui->BL_PRM_page){
        ui->BL_PRM_right->click();
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

// add a number to line edit
void addnle(QLineEdit* line, int number){
    line->setText(line->text() + QString::number(number));
}

void MainWindow::on_number_i_clicked(int i)
{
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

    if(ui->modals->currentWidget() == ui->password){

        ui->password->setProperty("password", ui->password->property("password").toString() + QString::number(i));
        ui->label_16->setText(ui->label_16->text() + "*");
        return;
    }

    auto curr = ui->mainPages->currentWidget();
    if(curr == ui->channel_editor_page){

        // tetra tmo
        if(ui->channel_editor_state->property("chosen") == 2){
            switch (curr_editor_field["tmo"]) {
            case 2:
                addnle(ui->tmo_mcc, i);
                break;
            case 3:
                addnle(ui->tmo_mnc, i);
                break;
            case 4:
                addnle(ui->tmo_gssi, i);
                break;
            }
            return;
        }

        // vpd
        if(ui->channel_editor_state->property("chosen") == 3){
            switch (curr_editor_field["vpd"]) {
            case 1:
                addnle(ui->vpd_mcc, i);
                break;
            case 2:
                addnle(ui->vpd_mnc, i);
                break;
            case 3:
                addnle(ui->vpd_gssi, i);
                break;
            case 4:
                addnle(ui->vpd_freq, i);
                break;
            }
            return;
        }

        // am25
        if(ui->channel_editor_state->property("chosen") == 4){
            switch (curr_editor_field["am25"]) {
            case 3:
                addnle(ui->am25_freq, i);
                break;
            case 4:
                addnle(ui->am25_prm_freq, i);
                break;
            case 5:
                addnle(ui->am25_prd_freq, i);
                break;
            }
            return;
        }

        // chm25
        if(ui->channel_editor_state->property("chosen") == 5){
            switch (curr_editor_field["chm25"]) {
            case 3:
                addnle(ui->chm25_freq, i);
                break;
            case 4:
                addnle(ui->chm25_prm_freq, i);
                break;
            case 5:
                addnle(ui->chm25_prd_freq, i);
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

        // chm50
        if(ui->channel_editor_state->property("chosen") == 6){
            switch (curr_editor_field["chm50"]) {
            case 3:
                addnle(ui->chm50_freq, i);
                break;
            case 4:
                addnle(ui->chm50_prm_freq, i);
                break;
            case 5:
                addnle(ui->chm50_prd_freq, i);
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

        // obp
        if(ui->channel_editor_state->property("chosen") == 7){
            switch (curr_editor_field["obp"]) {
            case 3:
                addnle(ui->obp_freq, i);
                break;
            }
            return;
        }

        // fm
        if(ui->channel_editor_state->property("chosen") == 8){
            switch (curr_editor_field["fm"]) {
            case 3:
                addnle(ui->fm_freq, i);
                break;
            case 4:
                addnle(ui->fm_prm_freq, i);
                break;
            case 5:
                addnle(ui->fm_prd_freq, i);
                break;
            }
            return;
        }
    }
}

void go_up(QListWidget* qlw, uint size){
    qlw->setCurrentRow((qlw->currentRow() - 1 + size) % size);
}

void MainWindow::clear_chm25_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->chm25_prd->setStyleSheet("background: white;");
    ui->chm25_dualfreq->setStyleSheet("background: white;");
    ui->chm25_freq_full->setStyleSheet("background: white;");
    ui->chm25_prm_freq_full->setStyleSheet("background: white;");
    ui->chm25_prd_freq_full->setStyleSheet("background: white;");
    ui->chm25_ctcss->setStyleSheet("background: white;");
    ui->chm25_name->setStyleSheet("background: white;");
    ui->label_32->setVisible(false);
    ui->label_42->setVisible(false);
    ui->label_39->setVisible(false);
}

void MainWindow::clear_chm25_d_fields(){
    ui->channel_in_dir_name->setStyleSheet("text-align: left;");

    ui->is_forbidden_prd_d->setStyleSheet("background: white;");
    ui->is_tone_call->setStyleSheet("background: white;");
    ui->scan->setStyleSheet("background: white;");
    ui->economizer->setStyleSheet("background: white;");

    ui->name_d->setStyleSheet("background: white;");
    ui->background_dir_picture->setStyleSheet("");
}

void MainWindow::clear_chm50_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->chm50_prd->setStyleSheet("background: white;");
    ui->chm50_dualfreq->setStyleSheet("background: white;");
    ui->chm50_freq_full->setStyleSheet("background: white;");
    ui->chm50_prm_freq_full->setStyleSheet("background: white;");
    ui->chm50_prd_freq_full->setStyleSheet("background: white;");
    ui->chm50_ctcss->setStyleSheet("background: white;");
    ui->chm50_name->setStyleSheet("background: white;");
    ui->label_88->setVisible(false);
    ui->label_85->setVisible(false);
    ui->label_82->setVisible(false);
}

void MainWindow::clear_am25_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->am25_prd->setStyleSheet("background: white;");
    ui->am25_dualfreq->setStyleSheet("background: white;");
    ui->am25_freq_full->setStyleSheet("background: white;");
    ui->am25_prm_freq_full->setStyleSheet("background: white;");
    ui->am25_prd_freq_full->setStyleSheet("background: white;");
    ui->am25_name->setStyleSheet("background: white;");
    ui->label_54->setVisible(false);
    ui->label_57->setVisible(false);
    ui->label_60->setVisible(false);
}

void MainWindow::clear_tetra_tmo_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->tmo_net->setStyleSheet("background: white;");
    ui->tmo_mcc->setStyleSheet("background: white;");
    ui->tmo_mnc->setStyleSheet("background: white;");
    ui->tmo_gssi->setStyleSheet("background: white;");
    ui->tmo_mask->setStyleSheet("background: white;");
    ui->tmo_name->setStyleSheet("background: white;");
    ui->tmo_vesh->setStyleSheet("background: white;");
}

void MainWindow::clear_vpd_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->vpd_mcc->setStyleSheet("background: white;");
    ui->vpd_mnc->setStyleSheet("background: white;");
    ui->vpd_gssi->setStyleSheet("background: white;");
    ui->vpd_freq_full->setStyleSheet("background: white;");
    ui->vpd_name->setStyleSheet("background: white;");
    ui->label_69->setVisible(false);
}

void MainWindow::clear_obp_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->obp_prd->setStyleSheet("background: white;");
    ui->obp_band->setStyleSheet("background: white;");
    ui->obp_freq_full->setStyleSheet("background: white;");
    ui->obp_name->setStyleSheet("background: white;");
    ui->label_72->setVisible(false);
}

void MainWindow::clear_fm_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->fm_prd->setStyleSheet("background: white;");
    ui->fm_dualfreq->setStyleSheet("background: white;");
    ui->fm_freq_full->setStyleSheet("background: white;");
    ui->fm_prm_freq_full->setStyleSheet("background: white;");
    ui->fm_prd_freq_full->setStyleSheet("background: white;");
    ui->fm_name->setStyleSheet("background: white;");
    ui->label_77->setVisible(false);
    ui->label_75->setVisible(false);
    ui->label_70->setVisible(false);
}

void MainWindow::update_channel_editor_page(){
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
        if(ui->channel_editor_states->currentWidget() != ui->TETRA_TMO_page)
            ui->channel_editor_states->setCurrentWidget(ui->TETRA_TMO_page);
        break;
    case 3:
        if(ui->channel_editor_states->currentWidget() != ui->VPD_page)
            ui->channel_editor_states->setCurrentWidget(ui->VPD_page);
        break;
    case 4:
        if(ui->channel_editor_states->currentWidget() != ui->AM25_page)
            ui->channel_editor_states->setCurrentWidget(ui->AM25_page);
        break;
    case 5:
        if(ui->channel_editor_states->currentWidget() != ui->CHM25_page)
            ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
        break;
    case 6:
        if(ui->channel_editor_states->currentWidget() != ui->CHM50_page)
            ui->channel_editor_states->setCurrentWidget(ui->CHM50_page);
        break;
    case 7:
        if(ui->channel_editor_states->currentWidget() != ui->OBP_page)
            ui->channel_editor_states->setCurrentWidget(ui->OBP_page);
        break;
    case 8:
        if(ui->channel_editor_states->currentWidget() != ui->FM_page)
            ui->channel_editor_states->setCurrentWidget(ui->FM_page);
        break;
    default:
        qCritical("crit: update_channel_editor_page: channel_editor_state");
    }

    // change buttons at type changing
    if(curr_editor_field[channel_types[ui->channel_editor_state->property("chosen").toInt()]] == 0){
        if(ui->modals->currentWidget() == ui->channel_type){
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
        ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
        return;
    }

    // tetra tmo
    if(ui->channel_editor_state->property("chosen") == 2){
        clear_tetra_tmo_fields();
        switch (curr_editor_field["tmo"]) {
        case 0:
            // was upper
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->tmo_net->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Выбрать");
            break;
        case 2:
            ui->tmo_mcc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 3:
            ui->tmo_mnc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->tmo_gssi->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->tmo_vesh->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 6:
            ui->tmo_mask->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 7:
            ui->tmo_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->tmo_name->setFocus();
            break;
        default:
            qCritical("tmo: update_channel_editor_page: no way");
        }

        return;
    }

    // vpd
    if(ui->channel_editor_state->property("chosen") == 3){
        clear_vpd_fields();
        switch (curr_editor_field["vpd"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->vpd_mcc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 2:
            ui->vpd_mnc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 3:
            ui->vpd_gssi->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->vpd_freq_full->setStyleSheet("#vpd_freq_full{ border: 2px solid black; background: white;}");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->vpd_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->vpd_name->setFocus();
            break;
        default:
            qCritical("vpd: update_channel_editor_page: no way");
        }

        if(curr_editor_field["vpd"] == 4){
            ui->label_69->setVisible(true);
        }
        else{
            ui->label_69->setVisible(false);
        }

        return;
    }

    // am25
    if(ui->channel_editor_state->property("chosen") == 4){
        clear_am25_fields();
        switch (curr_editor_field["am25"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->am25_prd->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->am25_dualfreq->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->am25_freq_full->setStyleSheet("#am25_freq_full {border: 2px solid black; background: white;}");
            ui->label_57->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->am25_prm_freq_full->setStyleSheet("#am25_prm_freq_full {border: 2px solid black; background: white;}");
            ui->label_60->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->am25_prd_freq_full->setStyleSheet("#am25_prd_freq_full {border: 2px solid black; background: white;}");
            ui->label_54->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->am25_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->am25_name->setFocus();
            break;
        default:
            qCritical("am25: update_channel_editor_page: no way");
        }

        if(ui->am25_dualfreq->isChecked()){
            ui->widget_21->setVisible(true);
            ui->widget_19->setVisible(true);
            ui->widget_20->setVisible(false);
        }
        else{
            ui->widget_21->setVisible(false);
            ui->widget_19->setVisible(false);
            ui->widget_20->setVisible(true);
        }
        return;
    }

    // chm25
    if(ui->channel_editor_state->property("chosen") == 5){
        clear_chm25_fields();
        switch (curr_editor_field["chm25"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->chm25_prd->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->chm25_dualfreq->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->chm25_freq_full->setStyleSheet("#chm25_freq_full {border: 2px solid black; background: white;}");
            ui->label_32->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->chm25_prm_freq_full->setStyleSheet("#chm25_prm_freq_full {border: 2px solid black; background: white;}");
            ui->label_42->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->chm25_prd_freq_full->setStyleSheet("#chm25_prd_freq_full {border: 2px solid black; background: white;}");
            ui->label_39->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->chm25_ctcss->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->ctcss){
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            else{
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->chm25_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->chm25_name->setFocus();
            break;
        default:
            qCritical("chm25: update_channel_editor_page: no way");
        }

        if(ui->chm25_dualfreq->isChecked()){
            ui->widget_6->setVisible(true);
            ui->widget_9->setVisible(true);
            ui->widget_4->setVisible(false);
        }
        else{
            ui->widget_6->setVisible(false);
            ui->widget_9->setVisible(false);
            ui->widget_4->setVisible(true);
        }
        return;
    }

    // chm50
    if(ui->channel_editor_state->property("chosen") == 6){
        clear_chm50_fields();
        switch (curr_editor_field["chm50"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->chm50_prd->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->chm50_dualfreq->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->chm50_freq_full->setStyleSheet("#chm50_freq_full {border: 2px solid black; background: white;}");
            ui->label_88->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->chm50_prm_freq_full->setStyleSheet("#chm50_prm_freq_full {border: 2px solid black; background: white;}");
            ui->label_85->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->chm50_prd_freq_full->setStyleSheet("#chm50_prd_freq_full {border: 2px solid black; background: white;}");
            ui->label_82->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->chm50_ctcss->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->ctcss){
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            else{
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->chm50_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->chm50_name->setFocus();
            break;
        default:
            qCritical("chm50: update_channel_editor_page: no way");
        }

        if(ui->chm50_dualfreq->isChecked()){
            ui->widget_27->setVisible(true);
            ui->widget_26->setVisible(true);
            ui->widget_28->setVisible(false);
        }
        else{
            ui->widget_27->setVisible(false);
            ui->widget_26->setVisible(false);
            ui->widget_28->setVisible(true);
        }
        return;
    }

    // obp
    if(ui->channel_editor_state->property("chosen") == 7){
        clear_obp_fields();
        switch (curr_editor_field["obp"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->obp_prd->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->obp_band->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->obp_freq_full->setStyleSheet("#obp_freq_full {border: 2px solid black; background: white;}");
            ui->label_72->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->obp_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->label_42->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->obp_name->setFocus();
            break;
        default:
            qCritical("obp: update_channel_editor_page: no way");
        }
        return;
    }

    // fm
    if(ui->channel_editor_state->property("chosen") == 8){
        clear_fm_fields();
        switch (curr_editor_field["fm"]) {
        case 0:
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->fm_prd->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 2:
            ui->fm_dualfreq->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            break;
        case 3:
            ui->fm_freq_full->setStyleSheet("#fm_freq_full {border: 2px solid black; background: white;}");
            ui->label_77->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 4:
            ui->fm_prm_freq_full->setStyleSheet("#fm_prm_freq_full {border: 2px solid black; background: white;}");
            ui->label_75->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 5:
            ui->fm_prd_freq_full->setStyleSheet("#fm_prd_freq_full {border: 2px solid black; background: white;}");
            ui->label_70->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            break;
        case 6:
            ui->fm_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->fm_name->setFocus();
            break;
        default:
            qCritical("fm: update_channel_editor_page: no way");
        }

        if(ui->fm_dualfreq->isChecked()){
            ui->widget_23->setVisible(true);
            ui->widget_22->setVisible(true);
            ui->widget_25->setVisible(false);
        }
        else{
            ui->widget_23->setVisible(false);
            ui->widget_22->setVisible(false);
            ui->widget_25->setVisible(true);
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
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white; text-align: left;");
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
            //ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white; text-align: left;");
            break;
        case 1:
            ui->is_forbidden_prd_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 2:
            ui->is_tone_call->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 3:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
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
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 5:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            break;
        case 6:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
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

void MainWindow::volume_show(){
    QTimer *t;
    timers.push(t = new QTimer());
    int i = ui->volume->property("clicked_times").toInt() + 1;
    ui->volume->setProperty("clicked_times", i);
    connect(t, &QTimer::timeout, this, [i, t, this](){ volume_handler(i); t->stop(); });
    t->start(1000);
}

void MainWindow::noise_show(){
    QTimer *t;
    timers.push(t = new QTimer());
    int i = ui->noise->property("clicked_times").toInt() + 1;
    ui->noise->setProperty("clicked_times", i);
    ui->main_left->setText("Каналы");
    ui->main_right->setText("Назад");
    connect(t, &QTimer::timeout, this, [i, t, this](){ noise_handler(i); t->stop(); });
    t->start(1000);
}

void MainWindow::volume_handler(int i){
    if(i == ui->volume->property("clicked_times").toInt()){
        if(ui->atuners->currentWidget() == ui->volume){
            ui->atuners->setCurrentWidget(ui->no_tuners);
            if(ui->volume->property("from_menu").toBool() == true){
                ui->volume->setProperty("from_menu", false);
                menu_screen();
            }
        }
    }
}

void MainWindow::noise_handler(int i){
    if(i == ui->noise->property("clicked_times").toInt()){
        if(ui->atuners->currentWidget() == ui->noise){
            ui->atuners->setCurrentWidget(ui->no_tuners);
            ui->main_left->setText("Меню");
            ui->main_right->setText("Направление");
            if(ui->noise->property("from_menu").toBool() == true){
                ui->noise->setProperty("from_menu", false);
                menu_screen();
            }
        }        
    }
}

void MainWindow::on_up_arrow_clicked()
{
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

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
    if(curr == ui->main_page){
        if(ui->atuners->currentWidget() == ui->no_tuners){
            ui->atuners->setCurrentWidget(ui->volume);
            volume_show();
            return;
        }
        if(ui->atuners->currentWidget() == ui->volume){
            volume++;
            if(volume > MAX_VOLUME) volume = MAX_VOLUME;
            ui->label_89->setStyleSheet("image: url(:/resources/volume_" + QString::number(volume) + ".png)");
            volume_show();
            return;
        }
        if(ui->atuners->currentWidget() == ui->noise){
            noise++;
            if(noise > MAX_NOISE) noise = MAX_NOISE;
            ui->label_92->setStyleSheet("image: url(:/resources/volume_" + QString::number(noise) + ".png)");
            noise_show();
            return;
        }
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
            if(ui->modals->currentWidget() == ui->channel_type){
                go_up(ui->ch_type_popup, 9);
                return;
            }
        }

        // none
        if(ui->channel_editor_state->property("chosen") == 0){
            return;
        }

        // tetra tmo
        if(ui->channel_editor_state->property("chosen") == 2){
            uint sz = editor_fields["tmo"].size();
            curr_editor_field["tmo"] = (curr_editor_field["tmo"] - 1 + sz) % sz;

            update_channel_editor_page();
            return;
        }

        // vpd
        if(ui->channel_editor_state->property("chosen") == 3){
            uint sz = editor_fields["vpd"].size();
            curr_editor_field["vpd"] = (curr_editor_field["vpd"] - 1 + sz) % sz;

            update_channel_editor_page();
            return;
        }

        // am25
        if(ui->channel_editor_state->property("chosen") == 4){
            uint sz = editor_fields["am25"].size();
            curr_editor_field["am25"] = (curr_editor_field["am25"] - 1 + sz) % sz;
            if(ui->am25_dualfreq->isChecked()){
                if(curr_editor_field["am25"] == 3) curr_editor_field["am25"]--;
            }
            else{
                if(curr_editor_field["am25"] == 5) curr_editor_field["am25"] -= 2;
            }
            update_channel_editor_page();
            return;
        }

        // chm25
        if(ui->channel_editor_state->property("chosen") == 5){
            if(curr_editor_field["chm25"] == 6){
                if(ui->modals->currentWidget() == ui->ctcss){
                    go_up(ui->ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm25"].size();
            curr_editor_field["chm25"] = (curr_editor_field["chm25"] - 1 + sz) % sz;
            if(ui->chm25_dualfreq->isChecked()){
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
                if(ui->modals->currentWidget() == ui->ctcss){
                    go_up(ui->ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm50"].size();
            curr_editor_field["chm50"] = (curr_editor_field["chm50"] - 1 + sz) % sz;
            if(ui->chm50_dualfreq->isChecked()){
                if(curr_editor_field["chm50"] == 3) curr_editor_field["chm50"]--;
            }
            else{
                if(curr_editor_field["chm50"] == 5) curr_editor_field["chm50"] -= 2;
            }
            update_channel_editor_page();
            return;
        }

        // obp
        if(ui->channel_editor_state->property("chosen") == 7){
            uint sz = editor_fields["obp"].size();
            curr_editor_field["obp"] = (curr_editor_field["obp"] - 1 + sz) % sz;
            update_channel_editor_page();
            return;
        }

        // fm
        if(ui->channel_editor_state->property("chosen") == 8){
            uint sz = editor_fields["fm"].size();
            curr_editor_field["fm"] = (curr_editor_field["fm"] - 1 + sz) % sz;
            if(ui->fm_dualfreq->isChecked()){
                if(curr_editor_field["fm"] == 3) curr_editor_field["fm"]--;
            }
            else{
                if(curr_editor_field["fm"] == 5) curr_editor_field["fm"] -= 2;
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
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

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
    if(curr == ui->main_page){
        if(ui->atuners->currentWidget() == ui->no_tuners){
            ui->atuners->setCurrentWidget(ui->noise);
            noise_show();
            return;
        }
        if(ui->atuners->currentWidget() == ui->volume){
            volume--;
            if(volume < MIN_VOLUME) volume = MIN_VOLUME;
            ui->label_89->setStyleSheet("image: url(:/resources/volume_" + QString::number(volume) + ".png)");
            volume_show();
            return;
        }
        if(ui->atuners->currentWidget() == ui->noise){
            noise--;
            if(noise < MIN_NOISE) noise = MIN_NOISE;
            ui->label_92->setStyleSheet("image: url(:/resources/volume_" + QString::number(noise) + ".png)");
            noise_show();
            return;
        }
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
            if(ui->modals->currentWidget() == ui->channel_type){
                go_down(ui->ch_type_popup, 9);
                return;
            }
        }

        // none
        if(ui->channel_editor_state->property("chosen") == 0){
            return;
        }

        // tetra tmo
        if(ui->channel_editor_state->property("chosen") == 2){
            uint sz = editor_fields["tmo"].size();
            curr_editor_field["tmo"] = (curr_editor_field["tmo"] + 1) % sz;

            update_channel_editor_page();
            return;
        }

        // vpd
        if(ui->channel_editor_state->property("chosen") == 3){
            uint sz = editor_fields["vpd"].size();
            curr_editor_field["vpd"] = (curr_editor_field["vpd"] + 1) % sz;

            update_channel_editor_page();
            return;
        }


        // am25
        if(ui->channel_editor_state->property("chosen") == 4){
            uint sz = editor_fields["am25"].size();
            curr_editor_field["am25"] = (curr_editor_field["am25"] + 1) % sz;
            if(ui->am25_dualfreq->isChecked()){
                if(curr_editor_field["am25"] == 3) curr_editor_field["am25"]++;
            }
            else{
                if(curr_editor_field["am25"] == 4) curr_editor_field["am25"] += 2;
            }

            update_channel_editor_page();
            return;
        }

        // chm25
        if(ui->channel_editor_state->property("chosen") == 5){

            if(curr_editor_field["chm25"] == 6){
                if(ui->modals->currentWidget() == ui->ctcss){
                    go_down(ui->ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm25"].size();
            curr_editor_field["chm25"] = (curr_editor_field["chm25"] + 1) % sz;
            if(ui->chm25_dualfreq->isChecked()){
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
                if(ui->modals->currentWidget() == ui->ctcss){
                    go_down(ui->ctcss_popup, 65);
                    return;
                }
            }

            uint sz = editor_fields["chm50"].size();
            curr_editor_field["chm50"] = (curr_editor_field["chm50"] + 1) % sz;
            if(ui->chm50_dualfreq->isChecked()){
                if(curr_editor_field["chm50"] == 3) curr_editor_field["chm50"]++;
            }
            else{
                if(curr_editor_field["chm50"] == 4) curr_editor_field["chm50"] += 2;
            }

            update_channel_editor_page();
            return;
        }


        // obp
        if(ui->channel_editor_state->property("chosen") == 7){
            uint sz = editor_fields["obp"].size();
            curr_editor_field["obp"] = (curr_editor_field["obp"] + 1) % sz;

            update_channel_editor_page();
            return;
        }

        // fm
        if(ui->channel_editor_state->property("chosen") == 8){
            uint sz = editor_fields["fm"].size();
            curr_editor_field["fm"] = (curr_editor_field["fm"] + 1) % sz;
            if(ui->fm_dualfreq->isChecked()){
                if(curr_editor_field["fm"] == 3) curr_editor_field["fm"]++;
            }
            else{
                if(curr_editor_field["fm"] == 4) curr_editor_field["fm"] += 2;
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

    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }
}

void MainWindow::on_chm25_dualfreq_clicked()
{
    if(ui->chm25_dualfreq->isChecked()){
        ui->widget_6->setVisible(true);
        ui->widget_9->setVisible(true);
        ui->widget_4->setVisible(false);
    }
    else{
        ui->widget_6->setVisible(false);
        ui->widget_9->setVisible(false);
        ui->widget_4->setVisible(true);
    }
}

void MainWindow::on_talk_button_pressed()
{

}

void MainWindow::check_holded_right_tube(int i){
    if(i == ui->right_tube->property("clicked_times").toInt()){

        // включение после секунды
        if(ui->mainPages->currentWidget() == ui->offscreen){
            ui->right_tube->setProperty("was_action", true);
            selfcontrol_screen();
            return;
        }

        if(ui->modals->currentWidget() == ui->no_modals){
            ui->right_tube->setProperty("was_action", true);
            ui->modals->setCurrentWidget(ui->shutdown);
            QTimer* t;
            timers.push(t = new QTimer());
            connect(t, &QTimer::timeout, this, [i, t, this]{ check_holded_right_tube(i); t->stop(); });
            t->start(1000);
            return;
        }

        if(ui->modals->currentWidget() == ui->shutdown){
            ui->right_tube->setProperty("was_action", true);
            ui->modals->setCurrentWidget(ui->no_modals);
            ui->mainPages->setCurrentWidget(ui->offscreen);
            return;
        }
    }
}

void MainWindow::on_right_tube_pressed()
{
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

    ui->right_tube->setProperty("was_action", false);

    auto curr = ui->mainPages->currentWidget();
    int i = ui->right_tube->property("clicked_times").toInt();
    QTimer* t;
    timers.push(t = new QTimer());
    connect(t, &QTimer::timeout, this, [i, t, this](){ this->check_holded_right_tube(i); t->stop(); });
    t->start(1000);
    return;
}

void MainWindow::on_right_tube_released()
{
    ui->right_tube->setProperty("clicked_times", ui->right_tube->property("clicked_times").toInt() + 1);
    if(ui->modals->currentWidget() == ui->shutdown){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

    if(ui->right_tube->property("was_action").toBool() == false){
        auto curr = ui->mainPages->currentWidget();
        if(curr == ui->loading_page){
            return;
        }
        if(curr == ui->main_page){
            return;
        }
        if(curr == ui->menu_page){
            main_screen();
            return;
        }
        if(curr == ui->service_menu_page){
            menu_screen();
            return;
        }
        if(curr == ui->data_editor_page){
            service_menu_screen();
            return;
        }
        if(curr == ui->channel_list_page){
            data_editor_screen();
            return;
        }
        if(curr == ui->direction_list_page){
            data_editor_screen();
            return;
        }
        if(curr == ui->direction_selection_page){
            // nothing
            //return;
        }
        if(curr == ui->channel_editor_page){
            if(ui->modals->currentWidget() == ui->no_modals){
                if(channel_map[selected_items["channel_list"]].channel->is_new){
                    delete_channel(selected_items["channel_list"]);
                }
                channel_list_screen();
                return;
            }
            ui->modals->setCurrentWidget(ui->no_modals);
            return;
        }
        if(curr == ui->direction_editor_page){
            return;
        }

        if(curr == ui->RS485_PRM_page){
            service_menu_screen();
            return;
        }
        if(curr == ui->RS485_PRD_page){
            service_menu_screen();
            ui->modals->setCurrentWidget(ui->no_modals);
            return;
        }
        if(curr == ui->USB_PRM_page){
            service_menu_screen();
            return;
        }
        if(curr == ui->BL_PRM_page){
            service_menu_screen();
            ui->modals->setCurrentWidget(ui->no_modals);
            return;
        }
        if(curr == ui->BL_PRD_page){
            service_menu_screen();
            ui->modals->setCurrentWidget(ui->no_modals);
            return;
        }
    }
}

void MainWindow::on_RS485_PRM_right_clicked()
{
    service_menu_screen();
}

void MainWindow::on_USB_PRM_right_clicked()
{
    service_menu_screen();
}

void MainWindow::on_RS485_PRD_right_clicked()
{
    service_menu_screen();
    ui->modals->setCurrentWidget(ui->no_modals);
}

void MainWindow::on_BL_PRM_right_clicked()
{
    ui->lineEdit->backspace();
}
