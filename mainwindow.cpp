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
    channels_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    channels_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    channels_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : channels_popup_menu_list_item){
        ui->channel_popup_menu_list->addItem(item);
    }

    selected_items["channel_popup_menu_list"] = channels_popup_menu_list_item[0];

    ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

    //
    directions_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    directions_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    directions_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : directions_popup_menu_list_item){
        ui->direction_popup_menu_list->addItem(item);
    }

    selected_items["direction_popup_menu_list"] = directions_popup_menu_list_item[0];

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

void MainWindow::update_channels_list_screen()
{
    if(selected_items["channels_list"] == nullptr){
        ui->channel_list_stackedWidget->setCurrentWidget(ui->empty_channel_list_page);
    }
    else{
        ui->channel_list_stackedWidget->setCurrentWidget(ui->nonempty_channel_list_page);
    }
}

void MainWindow::channels_list_screen()
{
    ui->mainPages->setCurrentWidget(ui->channels_list_page);
    update_channels_list_screen();
    ui->channels_list->setCurrentItem(selected_items["channels_list"]);
}

void MainWindow::directions_list_screen()
{
    ui->mainPages->setCurrentWidget(ui->directions_list_page);
    ui->directions_list->setCurrentItem(selected_items["directions_list"]);
}

void MainWindow::on_data_editor_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == data_editor_list_item[1]){
        directions_list_screen();
    }
    if(item == data_editor_list_item[2]){
        channels_list_screen();
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

void MainWindow::on_channels_list_right_clicked()
{
    data_editor_screen();
}

void MainWindow::on_directions_list_right_clicked()
{
    data_editor_screen();
}

void MainWindow::on_data_editor_left_clicked()
{
    QString s = "item: " + selected_items["data_editor_list"]->text();
    on_data_editor_list_itemDoubleClicked(selected_items["data_editor_list"]);
}

void MainWindow::on_data_editor_right_clicked()
{
    service_menu_screen();
}

void MainWindow::on_channel_list_left_clicked()
{
    ui->channel_popup_menu->setEnabled(!ui->channel_popup_menu->isVisible());
    ui->channel_popup_menu->setVisible(!ui->channel_popup_menu->isVisible());
}

void MainWindow::on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

    if(item == channels_popup_menu_list_item[0]){
        if(selected_items["channels_list"] == nullptr) return;
        channel_editor_screen();
    }
    if(item == channels_popup_menu_list_item[1]){
        QListWidgetItem* ref = new QListWidgetItem(QIcon(""), "");
        QListWidgetItem* ref2 = new QListWidgetItem(QIcon(""), "");
        Channel* new_ch = new Channel();
        channels_map[ref] = {new_ch, ref2};
        channels_map_d[ref2] = new_ch;

        // for channels
        ui->channels_list->addItem(ref);
        // for directions
        ui->channel_choice_list->addItem(ref2);

        selected_items["channels_list"] = ref;
        channel_editor_screen();
    }
    if(item == channels_popup_menu_list_item[2]){
        if(selected_items["channels_list"] != nullptr){

            // for channels
            ui->channels_list->removeItemWidget(selected_items["channels_list"]);
            // for directions
            ui->channel_choice_list->removeItemWidget(channels_map[selected_items["channels_list"]].ref2);
            channels_map_d.erase(channels_map[selected_items["channels_list"]].ref2);

            delete channels_map[selected_items["channels_list"]].channel;
            delete channels_map[selected_items["channels_list"]].ref2;
            channels_map.erase(selected_items["channels_list"]);
            delete selected_items["channels_list"];
            selected_items["channels_list"] = channels_map.empty() ? nullptr : channels_map.begin()->first;

            update_channels_list_screen();
        }
    }
}

// channel loading
void MainWindow::channel_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->channel_editor_page);

    Channel* curr = channels_map[selected_items["channels_list"]].channel;

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

void MainWindow::on_channel_editor_back_clicked()
{
    //channels_list_screen();
}

// channel saving
void MainWindow::on_pushButton_clicked()
{
    Channel* curr = channels_map[selected_items["channels_list"]].channel;
    curr->state = ui->channel_editor_state->currentIndex();
    if(curr->state == 0){
        curr->PRD = false;
        curr->dualfreq = false;
        curr->freq = 0;
        curr->ctcss = 0;
        curr->name = "";
        selected_items["channels_list"]->setText("");
        channels_map[selected_items["channels_list"]].ref2->setText("");
    }
    if(curr->state == 5){
        curr->PRD = ui->is_forbidden_prd->isChecked();
        curr->dualfreq = ui->dualfreq->isChecked();
        curr->freq = (uint32_t)ui->channel_freq->text().toInt();
        curr->ctcss = ui->ctcss->currentIndex();
        curr->name = ui->channel_name->text();
        selected_items["channels_list"]->setText(curr->name);
        channels_map[selected_items["channels_list"]].ref2->setText(curr->name);
    }
    channels_list_screen();
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

void MainWindow::on_channels_list_itemClicked(QListWidgetItem *item)
{
    selected_items["channels_list"] = item;
}

void MainWindow::on_direction_list_left_clicked()
{
    ui->direction_popup_menu->setEnabled(!ui->direction_popup_menu->isVisible());
    ui->direction_popup_menu->setVisible(!ui->direction_popup_menu->isVisible());
}

void MainWindow::on_direction_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    ui->direction_popup_menu->setEnabled(false);
    ui->direction_popup_menu->setVisible(false);

    if(item == directions_popup_menu_list_item[0]){
        if(selected_items["directions_list"] == nullptr) return;
        direction_editor_screen();
    }
    if(item == directions_popup_menu_list_item[1]){
        QListWidgetItem* ref = new QListWidgetItem(QIcon(""), "");
        QListWidgetItem* ref2 = new QListWidgetItem(QIcon(""), "");
        Direction* new_dir = new Direction();
        directions_map[ref] = {new_dir, ref2};
        directions_map_d[ref2] = new_dir;

        ui->directions_list->addItem(ref);
        ui->directions_selection_list->addItem(ref2);

        selected_items["directions_list"] = ref;
        direction_editor_screen();
    }
    if(item == directions_popup_menu_list_item[2]){
        if(selected_items["directions_list"] != nullptr){
            ui->directions_list->removeItemWidget(selected_items["directions_list"]);
            ui->directions_selection_list->removeItemWidget(directions_map[selected_items["directions_list"]].ref2);
            directions_map_d.erase(directions_map[selected_items["directions_list"]].ref2);

            if(current_direction == directions_map[selected_items["directions_list"]].direction){
                current_direction = nullptr;
            }
            delete directions_map[selected_items["directions_list"]].direction;
            delete directions_map[selected_items["directions_list"]].ref2;
            directions_map.erase(selected_items["directions_list"]);
            delete selected_items["directions_list"];
            selected_items["directions_list"] = directions_map.empty() ? nullptr : directions_map.begin()->first;
        }
    }
}

// direction loading
void MainWindow::direction_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_editor_page);

    Direction* curr = directions_map[selected_items["directions_list"]].direction;

    if(curr->ch == nullptr){
        ui->channel_in_dir_name->setText("Не задано(Idle)");
        ui->channel_choice_list->setCurrentRow(-1);
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
    }
    else{
        ui->channel_in_dir_name->setText(curr->ch->name);
        for(const auto& p : channels_map_d){
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
    directions_map[selected_items["directions_list"]].direction->ch = channels_map_d[item];
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
    ui->channel_in_dir_name->setText(channels_map_d[item]->name);
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
    Direction* curr = directions_map[selected_items["directions_list"]].direction;

    curr->PRD = ui->is_forbidden_prd_d->isChecked();
    curr->tone_call = ui->is_tone_call->isChecked();
    curr->scan_list = ui->scan_list->currentIndex();
    curr->economizer = ui->economizer->currentIndex();
    curr->name = ui->name_d->text();
    curr->background = ui->background_dir_picture->currentIndex();
    selected_items["directions_list"]->setText(curr->name + "\n" + curr->ch->name);
    selected_items["directions_list"]->setIcon(QIcon(":/resources/picture32.png"));
    directions_map[selected_items["directions_list"]].ref2->setText(curr->name); // + "\n" + curr->ch->name
    directions_map[selected_items["directions_list"]].ref2->setIcon(QIcon(":/resources/picture32.png"));
    directions_list_screen();
}

void MainWindow::on_channel_in_dir_name_clicked()
{
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);
}

void MainWindow::on_channel_choice_list_itemClicked(QListWidgetItem *item)
{
    selected_items["channels_choice_list"] = item;
}

void MainWindow::on_directions_list_itemClicked(QListWidgetItem *item)
{
    selected_items["directions_list"] = item;
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
    if(selected_items["directions_selection_list"] != nullptr){
        current_direction = directions_map_d[selected_items["directions_selection_list"]];
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
    if(selected_items["directions_selection_list"] == nullptr){
        selected_items["directions_selection_list"] = directions_map_d.empty() ? nullptr : directions_map_d.begin()->first;
    }
}

void MainWindow::on_directions_selection_list_itemClicked(QListWidgetItem *item)
{
    selected_items["directions_selection_list"] = item;
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

void MainWindow::on_directions_button_clicked()
{
    direction_selection_screen();
}

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
}

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
        ui->menu_list->setCurrentRow((ui->menu_list->currentRow() - 1 + menu_list_size) % menu_list_size);
        return;
    }
    if(curr == ui->service_menu_page){
        ui->service_menu_list->setCurrentRow((ui->service_menu_list->currentRow() - 1 + service_menu_list_size) % service_menu_list_size);
        return;
    }
    if(curr == ui->data_editor_page){
        ui->data_editor_list->setCurrentRow((ui->data_editor_list->currentRow() - 1 + data_editor_list_size) % data_editor_list_size);
        return;
    }
}

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
        ui->menu_list->setCurrentRow((ui->menu_list->currentRow() + 1) % menu_list_size);
        return;
    }
    if(curr == ui->service_menu_page){
        ui->service_menu_list->setCurrentRow((ui->service_menu_list->currentRow() + 1) % service_menu_list_size);
        return;
    }
    if(curr == ui->data_editor_page){
        ui->data_editor_list->setCurrentRow((ui->data_editor_list->currentRow() + 1) % data_editor_list_size);
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
