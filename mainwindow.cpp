#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QDate>
#include <QDebug>
#include <QtMultimedia>
#include <iostream>

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
    ui->seconds->setText(s);

    ui->hours_minutes_2->setText(hm);
    ui->seconds_2->setText(s);

    ui->data_label->setText(QDate::currentDate().toString("dd.MM.yyyy"));
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
    menu_list_item[2] = new QListWidgetItem(QIcon(":/resources/picture32.png"), "Навигация");
    menu_list_item[3] = new QListWidgetItem(QIcon(":/resources/download32.png"), "Принятые сообщения");
    menu_list_item[4] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Настройки");
    menu_list_item[5] = new QListWidgetItem(QIcon(":/resources/settings32.png"), "Сервисное меню");
    menu_list_item[6] = new QListWidgetItem(QIcon(":/resources/battery32.png"), "Батарея");

    for(QListWidgetItem* item : menu_list_item){
        ui->menu_list->addItem(item);        
    }

    selected_items["menu_list"] = menu_list_item[0];

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

    selected_items["service_menu_list"] = service_menu_list_item[0];

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

    selected_items["data_editor_list"] = data_editor_list_item[0];

    //
    channel_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    channel_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    channel_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : channel_popup_menu_list_item){
        ui->channel_popup_menu_list->addItem(item);
    }

    selected_items["channel_popup_menu_list"] = channel_popup_menu_list_item[0];

    ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

    //
    direction_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    direction_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    direction_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : direction_popup_menu_list_item){
        ui->direction_popup_menu_list->addItem(item);
    }

    selected_items["direction_popup_menu_list"] = direction_popup_menu_list_item[0];

    ui->direction_popup_menu->setEnabled(false);
    ui->direction_popup_menu->setVisible(false);

    ui->scan_list->addItem("Нет");
    for (int i = 1; i < 33; i++) {
        ui->scan_list->addItem(QString::number(i));
    }

    for (int i = 0; i < 4; i++) {
        ui->economizer->addItem(QString::number(i));
    }
    for (int i = 0; i < 6; i++) {
        ui->background_dir_picture->addItem(QString::number(i));
    }

    broadcast_init();
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
            " border-image: url(:/resources/Kremlin.png)"
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
    ui->menu_list->setCurrentItem(selected_items["menu_list"]);
}

void MainWindow::service_menu_screen()
{
    ui->mainPages->setCurrentWidget(ui->service_menu_page);
    ui->service_menu_list->setCurrentItem(selected_items["service_menu_list"]);
}

void MainWindow::on_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == menu_list_item[5]){
        service_menu_screen();
    }
}

void MainWindow::on_menu_list_itemSelectionChanged()
{
    selected_items["menu_list"]->setBackground(QColor(255, 255, 255));
    selected_items["menu_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["menu_list"] = ui->menu_list->currentItem();

    selected_items["menu_list"]->setBackground(QColor(0, 64, 190));
    selected_items["menu_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_service_menu_list_itemSelectionChanged()
{
    selected_items["service_menu_list"]->setBackground(QColor(255, 255, 255));
    selected_items["service_menu_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["service_menu_list"] = ui->service_menu_list->currentItem();

    selected_items["service_menu_list"]->setBackground(QColor(56, 82, 130));
    selected_items["service_menu_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_data_editor_list_itemSelectionChanged()
{
    selected_items["data_editor_list"]->setBackground(QColor(255, 255, 255));
    selected_items["data_editor_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["data_editor_list"] = ui->data_editor_list->currentItem();

    selected_items["data_editor_list"]->setBackground(QColor(56, 82, 130));
    selected_items["data_editor_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_channel_popup_menu_list_itemSelectionChanged()
{
    selected_items["channel_popup_menu_list"]->setBackground(QColor(255, 255, 255));
    selected_items["channel_popup_menu_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["channel_popup_menu_list"] = ui->channel_popup_menu_list->currentItem();

    selected_items["channel_popup_menu_list"]->setBackground(QColor(56, 82, 130));
    selected_items["channel_popup_menu_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_direction_popup_menu_list_itemSelectionChanged()
{
    selected_items["direction_popup_menu_list"]->setBackground(QColor(255, 255, 255));
    selected_items["direction_popup_menu_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["direction_popup_menu_list"] = ui->direction_popup_menu_list->currentItem();

    selected_items["direction_popup_menu_list"]->setBackground(QColor(56, 82, 130));
    selected_items["direction_popup_menu_list"]->setTextColor(QColor(255, 255 ,255));
}

void MainWindow::on_direction_selection_list_itemSelectionChanged()
{
    selected_items["direction_selection_list"]->setBackground(QColor(255, 255, 255));
    selected_items["direction_selection_list"]->setTextColor(QColor(133, 165, 200));

    selected_items["direction_selection_list"] = ui->direction_selection_list->currentItem();

    selected_items["direction_selection_list"]->setBackground(QColor(56, 82, 130));
    selected_items["direction_selection_list"]->setTextColor(QColor(255, 255 ,255));
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
    ui->data_editor_list->setCurrentItem(selected_items["data_editor_list"]);
}

void MainWindow::update_channel_list_screen()
{
    if(selected_items["channel_list"] == nullptr){
        ui->empty_channel_list_label->setVisible(true);
        ui->empty_channel_list_label->setEnabled(true);
    }
    else{
        ui->empty_channel_list_label->setVisible(false);
        ui->empty_channel_list_label->setEnabled(false);
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
    ui->channel_list->setCurrentItem(selected_items["channel_list"]);
}

void MainWindow::direction_list_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_list_page);
    update_direction_list_screen();
    ui->direction_list->setCurrentItem(selected_items["direction_list"]);
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
        ui->channel_popup_menu->setEnabled(false);
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
        ui->direction_popup_menu->setEnabled(false);
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
        ui->channel_popup_menu->setEnabled(true);
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
        ui->channel_popup_menu->setEnabled(false);
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

        selected_items["channel_list"] = ref;

        ui->channel_popup_menu->setEnabled(false);
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
            delete selected_items["channel_list"];
            selected_items["channel_list"] = channel_map.empty() ? nullptr : channel_map.begin()->first;

            ui->channel_popup_menu->setEnabled(false);
            ui->channel_popup_menu->setVisible(false);
            ui->channel_list_left->setText("Меню");
            update_channel_list_screen();
        }
    }
}

// channel loading
void MainWindow::channel_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->channel_editor_page);

    Channel* curr = channel_map[selected_items["channel_list"]].channel;

    if(curr->state == 0){
        ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
    }
    if(curr->state == 5){
        ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
    }

    ui->channel_editor_state->setCurrentIndex(curr->state);
    ui->is_forbidden_prd->setCheckState(curr->PRD ? Qt::Checked : Qt::Unchecked);
    ui->dualfreq->setCheckState(curr->dualfreq ? Qt::Checked : Qt::Unchecked);
    ui->channel_freq->setText(QString::number(curr->freq));
    ui->ctcss->setCurrentIndex(curr->ctcss);
    ui->channel_name->setText(curr->name);
}

void MainWindow::on_channel_editor_right_clicked()
{
    //channel_list_screen();
    //TODO
}

// channel saving
void MainWindow::on_channel_editor_left_clicked()
{
    //TODO: change
    Channel* curr = channel_map[selected_items["channel_list"]].channel;
    curr->state = ui->channel_editor_state->currentIndex();
    if(curr->state == 0){
        curr->PRD = false;
        curr->dualfreq = false;
        curr->freq = 0;
        curr->ctcss = 0;
        curr->name = "";
        selected_items["channel_list"]->setText("");
        channel_map[selected_items["channel_list"]].ref2->setText("");
    }
    if(curr->state == 5){
        curr->PRD = ui->is_forbidden_prd->isChecked();
        curr->dualfreq = ui->dualfreq->isChecked();
        curr->freq = (uint32_t)ui->channel_freq->text().toInt();
        curr->ctcss = ui->ctcss->currentIndex();
        curr->name = ui->channel_name->text();
        selected_items["channel_list"]->setText(curr->name);
        channel_map[selected_items["channel_list"]].ref2->setText(curr->name);
    }
    channel_list_screen();
}

void MainWindow::on_channel_editor_state_currentIndexChanged(int index)
{
    if(index == 5){
        ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
    }
    else {
        ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
    }
}

void MainWindow::on_channel_list_itemClicked(QListWidgetItem *item)
{
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
        ui->direction_popup_menu->setEnabled(true);
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
        ui->direction_popup_menu->setEnabled(false);
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

        selected_items["direction_list"] = ref;
        ui->direction_popup_menu->setEnabled(false);
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
            selected_items["direction_list"] = direction_map.empty() ? nullptr : direction_map.begin()->first;
            ui->direction_popup_menu->setEnabled(false);
            ui->direction_popup_menu->setVisible(false);
            ui->direction_list_left->setText("Меню");
            //TODO ~update_direction_list
        }
    }
}

// direction loading
void MainWindow::direction_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_editor_page);

    Direction* curr = direction_map[selected_items["direction_list"]].direction;

    if(curr->ch == nullptr){
        ui->channel_in_dir_name->setText("Не задано(Idle)");
        ui->channel_choice_list->setCurrentRow(-1);
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
    }
    else{
        ui->channel_in_dir_name->setText(curr->ch->name);
        for(const auto& p : channel_map_d){
            if(p.second == curr->ch){
                ui->channel_choice_list->setCurrentItem(p.first);
                break;
            }
        }
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
    }

    ui->is_forbidden_prd_d->setCheckState(curr->PRD ? Qt::Checked : Qt::Unchecked);
    ui->is_tone_call->setCheckState(curr->tone_call ? Qt::Checked : Qt::Unchecked);
    ui->scan_list->setCurrentIndex(curr->scan_list);
    ui->economizer->setCurrentIndex(curr->economizer);
    ui->name_d->setText(curr->name);
    ui->background_dir_picture->setCurrentIndex(curr->background);
}

void MainWindow::on_channel_choice_list_itemDoubleClicked(QListWidgetItem *item)
{
    direction_map[selected_items["direction_list"]].direction->ch = channel_map_d[item];
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
    ui->channel_in_dir_name->setText(channel_map_d[item]->name);
}

void MainWindow::on_economizer_currentIndexChanged(int index)
{
    switch (index) {
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

// direction saving
void MainWindow::on_direction_editor_left_clicked()
{
    Direction* curr = direction_map[selected_items["direction_list"]].direction;

    curr->PRD = ui->is_forbidden_prd_d->isChecked();
    curr->tone_call = ui->is_tone_call->isChecked();
    curr->scan_list = ui->scan_list->currentIndex();
    curr->economizer = ui->economizer->currentIndex();
    curr->name = ui->name_d->text();
    curr->background = ui->background_dir_picture->currentIndex();
    selected_items["direction_list"]->setText(curr->name + "\n" + curr->ch->name);
    selected_items["direction_list"]->setIcon(QIcon(":/resources/picture32.png"));
    direction_map[selected_items["direction_list"]].ref2->setText(curr->name); // + "\n" + curr->ch->name
    direction_map[selected_items["direction_list"]].ref2->setIcon(QIcon(":/resources/picture32.png"));
    direction_list_screen();
}

void MainWindow::on_channel_in_dir_name_clicked()
{
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);
}

void MainWindow::on_channel_choice_list_itemClicked(QListWidgetItem *item)
{
    selected_items["channel_choice_list"] = item;
}

void MainWindow::on_direction_list_itemClicked(QListWidgetItem *item)
{
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
    //TODO
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
                selected_items["direction_selection_list"] = nullptr;
        }
        else{
            selected_items["direction_selection_list"] = direction_map_d.begin()->first;
            ui->direction_selection_list->setCurrentItem(selected_items["direction_selection_list"]);
        }
    }
}

void MainWindow::on_direction_selection_list_itemClicked(QListWidgetItem *item)
{
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
// Warning: where is it from??? mb main_right_clicked() ?
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
}

/*
                   \\\\\
            ========\\\\\
            ========/////
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
    // TODO: ..
}

void go_up(QListWidget* qlw, uint size){
    qlw->setCurrentRow((qlw->currentRow() - 1 + size) % size);
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
        //TODO
        return;
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
        //TODO
        return;
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
