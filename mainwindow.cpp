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

void MainWindow::update_channels_list_screen(){
    if(selected_items["channels_list"] == nullptr){
        ui->channel_list_stackedWidget->setCurrentWidget(ui->empty_channel_list_page);
    }
    else{
        ui->channel_list_stackedWidget->setCurrentWidget(ui->nonempty_channel_list_page);
    }
}

void MainWindow::channels_list_screen(){
    ui->mainPages->setCurrentWidget(ui->channels_list_page);
    update_channels_list_screen();
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

void MainWindow::on_channels_list_right_clicked()
{
    data_editor_screen();
}

void MainWindow::on_directions_list_right_clicked()
{
    data_editor_screen();
}

void MainWindow::on_data_editor_back_clicked()
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
void MainWindow::channel_editor_screen(){
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
        auto ref = new QListWidgetItem(QIcon(""), "");
        directions_map[ref] = new Direction();
        ui->directions_list->addItem(ref);
        selected_items["directions_list"] = ref;
        direction_editor_screen();
    }
    if(item == directions_popup_menu_list_item[2]){
        if(selected_items["directions_list"] != nullptr){
            ui->directions_list->removeItemWidget(selected_items["directions_list"]);
            directions_map.erase(selected_items["directions_list"]);
            delete selected_items["directions_list"];
            selected_items["directions_list"] = directions_map.empty() ? nullptr : directions_map.begin()->first;
        }
    }
}

// direction loading
void MainWindow::direction_editor_screen(){
    ui->mainPages->setCurrentWidget(ui->direction_editor_page);

    Direction* curr = directions_map[selected_items["directions_list"]];

    if(curr->ch == nullptr){
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->empty_direction_editor_page);
    }
    else{
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
    directions_map[selected_items["directions_list"]]->ch = channels_map_d[item];
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

}

void MainWindow::on_channel_in_dir_name_clicked()
{
    ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);
}
