#include "mainwindow.h"
#include "ui_mainwindow.h"

inline void delay(int millisecondsWait)
{
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(millisecondsWait);
    loop.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    channels_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    channels_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    channels_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(QListWidgetItem* item : channels_popup_menu_list_item){
        ui->channel_popup_menu_list->addItem(item);
    }

    selected_items["channels_popup_menu_list"] = channels_popup_menu_list_item[0];

    ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selfcontrol_screen(){
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

void MainWindow::loading_screen(){
    ui->mainPages->setCurrentWidget(ui->loading_page);

    // kinda choice
    delay(2000);

    menu_screen();
}

void MainWindow::main_screen(){
    ui->mainPages->setCurrentWidget(ui->main_page);
    delay(2000);

    // переключение
}

void MainWindow::menu_screen(){
    ui->mainPages->setCurrentWidget(ui->menu_page);
    ui->menu_list->setCurrentItem(selected_items["menu_list"]);
}

void MainWindow::service_menu_screen(){
    ui->mainPages->setCurrentWidget(ui->service_menu_page);
    ui->service_menu_list->setCurrentItem(selected_items["service_menu_list"]);
}

void MainWindow::on_menu_list_itemDoubleClicked(QListWidgetItem *item){
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

void MainWindow::on_service_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    if(item == service_menu_list_item[7]){
        data_editor_screen();
    }
}

void MainWindow::data_editor_screen(){
    ui->mainPages->setCurrentWidget(ui->data_editor_page);
    ui->data_editor_list->setCurrentItem(selected_items["data_editor_list"]);
}

void MainWindow::channels_list_screen(){
    ui->mainPages->setCurrentWidget(ui->channels_list_page);
    ui->channels_list->setCurrentItem(selected_items["channels_list"]);
}

void MainWindow::directions_list_screen(){
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

void MainWindow::on_service_menu_back_clicked()
{
    menu_screen();
}

void MainWindow::on_channels_list_back_clicked()
{
    data_editor_screen();
}

void MainWindow::on_directions_list_back_clicked()
{
    data_editor_screen();
}

void MainWindow::on_data_editor_back_clicked()
{
    service_menu_screen();
}

void MainWindow::on_channel_menu_button_clicked()
{
    if(ui->channel_popup_menu->isVisible()){
        ui->channel_popup_menu->setEnabled(false);
        ui->channel_popup_menu->setVisible(false);
    }
    else{
        ui->channel_popup_menu->setEnabled(true);
        ui->channel_popup_menu->setVisible(true);
    }
}

void MainWindow::on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    ui->channel_popup_menu->setEnabled(false);
    ui->channel_popup_menu->setVisible(false);

    if(item == channels_popup_menu_list_item[0]){
        if(current_channel == -1) return;
        channel_editor_screen(current_channel);
    }
    if(item == channels_popup_menu_list_item[1]){
        channels_list_item.push_back(std::make_pair(new QListWidgetItem(QIcon(""), ""), new Channel()));
        channel_editor_screen(channels_list_item.size() - 1);
    }
    if(item == channels_popup_menu_list_item[2]){
        if(current_channel != -1){
            channels_list_item.erase(channels_list_item.begin() + current_channel);
            current_channel--;
        }
    }
}

void MainWindow::channel_editor_screen(uint32_t ch){
    ui->mainPages->setCurrentWidget(ui->channel_editor_page);

    //ui->channel_editor_state->currentIndex()
    Channel* curr = channels_list_item[ch].second;
    if(curr->state == 0){
        ui->channel_editor_states->setCurrentWidget(ui->empty_state_page);
    }
    if(curr->state == 5){
        ui->channel_editor_states->setCurrentWidget(ui->CHM25_page);
        ui->is_forbidden_prd->setTristate(curr->PRD);
        ui->dualfreq->setTristate(curr->dualfreq);
        ui->channel_freq->setText(QString::number(curr->freq));
        ui->ctcss->setCurrentIndex(curr->ctcss);
        ui->channel_name->setText(curr->name);
    }
}

void MainWindow::on_channel_editor_back_clicked()
{
    channels_list_screen();
}

void MainWindow::on_pushButton_clicked()
{
    Channel* curr = channels_list_item[0].second;
    curr->state = ui->channel_editor_state->currentIndex();
    if(curr->state == 5){
        curr->PRD = ui->is_forbidden_prd->isTristate();
        curr->dualfreq = ui->dualfreq->isTristate();
        curr->freq = (uint32_t)ui->channel_freq->text().toInt();
        curr->ctcss = ui->ctcss->currentIndex();
        curr->name = ui->channel_name->text();
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
