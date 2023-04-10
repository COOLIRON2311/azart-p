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
#include <sstream>
#include <set>
#include <iomanip>
#include <QSizePolicy>
#include <QLineEdit>

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

    ui->label_144->setText(hm);
}

QPoint MainWindow::global_pos(QWidget* w){
    while(w != this){
        return global_pos(w->parentWidget()) + w->pos();
    }
    return QPoint(0, 0);
}

void MainWindow::readIP()
{
    QFile file("ip.txt");
//    qDebug() << QDir::currentPath();
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    ADDR = in.readLine().trimmed();
    file.close();
    qDebug() << ADDR;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readIP();
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


    //                0       1      2      3      4       5        6        7      8
    channel_types = {"none", "dmo", "tmo", "vpd", "am25", "chm25", "chm50", "obp", "fm"};

    //                         0
    editor_fields["none"] = { "type" };
    curr_editor_field["none"] = 0;
    //                        0       1        2       3                4           5      6      7       8      9        10        11        12         13          14         15        16        17         18         19        20      21          22
    editor_fields["dmo"] = { "type", "pprch", "retr", "prd_only_retr", "dualfreq", "mcc", "mnc", "gssi", "sos", "f_dmo", "f_retr", "tx_dmo", "chp_dmo", "chp_retr", "chp_prd", "prm_hz", "prd_hz", "prm_net", "prd_net", "n_retr", "mask", "mask_key", "name" };
    curr_editor_field["dmo"] = 0;
    //                        0       1      2      3      4       5       6       7           8
    editor_fields["tmo"] = { "type", "net", "mcc", "mnc", "gssi", "vesh", "mask", "mask_key", "name" };
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

    //                        0          1      2       3           4       5          6             7       8
    editor_fields["dir"] = { "channel", "prd", "tone", "priority", "scan", "timeout", "economizer", "name", "background"  };
    curr_editor_field["dir"] = 0;

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

    ui->scan->setProperty("chosen", 0);

    ui->scan_popup->addItem("Нет");
    for(int i = 1; i <= 32; i++){
       ui->scan_popup->addItem(QString::number(i));
    }

    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Карта"));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Отправить координаты"));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Запросить координаты"));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Сохранить точку"));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Сохраненные точки..."));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Точка маршрута..."));
    ui->nav_menu_list->addItem(new QListWidgetItem(QIcon(""), "Настройки"));

    ui->nav_menu_list->setCurrentRow(0);

    show3d = new QAction("3D", this);
    connect(show3d, &QAction::triggered, this, &MainWindow::show_3d);
    ui->menuBar->addAction(show3d);

    showrd = new QAction("Радиоданные", this);
    connect(showrd, &QAction::triggered, this, &MainWindow::show_radiodata);
    ui->menuBar->addAction(showrd);

    shownorm = new QAction("Нормативы", this);
    connect(shownorm, &QAction::triggered, this, &MainWindow::show_normative);
    ui->menuBar->addAction(shownorm);

    showrules = new QAction("Правила ведения переговоров", this);
    connect(showrules, &QAction::triggered, this, &MainWindow::show_rules);
    ui->menuBar->addAction(showrules);

    broadcast_init();

    ui->modals->setCurrentWidget(ui->no_modals);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(1);
    effect->setXOffset(2);
    effect->setYOffset(2);
    effect->setColor(Qt::black);
    ui->widget_18->setGraphicsEffect(effect);
    ui->widget_29->setGraphicsEffect(effect);
    ui->widget_30->setGraphicsEffect(effect);
    ui->widget_61->setGraphicsEffect(effect);

    ui->modals->move(248, 210);
    ui->atuners->move(248, 210);

    ui->label_107->setVisible(false);
    ui->obp_band->setProperty("band", 1);

    dmo_fields.append(QList<QWidget*>{
                          ui->set_state, //channel_editor_state
                          ui->widget_60, //ui->dmo_pprch
                          ui->widget_54, //ui->dmo_f_retr
                          ui->widget_55, //ui->dmo_prd_only_retr
                          ui->widget_56, //ui->dmo_dualfreq
                          ui->widget_43, //ui->dmo_mcc
                          ui->widget_44, //ui->dmo_mnc
                          ui->widget_42, //ui->dmo_gssi
                          ui->widget_45, //ui->dmo_sos
                          ui->widget_39, //ui->dmo_f_dmo
                          ui->widget_40, //ui->dmo_f_retr
                          ui->widget_48, //ui->dmo_tx_dmo
                          ui->widget_41, //ui->dmo_chp_dmo
                          ui->widget_47, //ui->dmo_chp_retr
                          ui->widget_59, //ui->dmo_chp_prd
                          ui->widget_49, //ui->dmo_prm_hz
                          ui->widget_50, //ui->dmo_prd_hz
                          ui->widget_51, //ui->dmo_prm_net
                          ui->widget_52, //ui->dmo_prd_net
                          ui->widget_46, //ui->dmo_n_retr
                          ui->widget_57, //ui->dmo_mask
                          ui->widget_53, //ui->dmo_mask_key
                          ui->widget_58, //ui->dmo_name
                      });

    direction_fields.append(
                QList<QWidget*>{
                    ui->widget_77,
                    ui->widget_67, // prd
                    ui->widget_68, // tone
                    ui->widget_74, // priority
                    ui->widget_69, // scan
                    ui->widget_73, // timeout
                    ui->widget_75, // eco
                    ui->widget_71, // name
                    ui->widget_76  // back
                        });

    for(int i = 1; i <= 32; i++){
        QWidget* ptr;
        freq_plans.push_back(ptr = new QWidget(ui->scrollAreaWidgetContents_2));

        //ptr->setMinimumSize(QSize(0, 25));

        auto horizontalLayout = new QHBoxLayout(ptr);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        auto label_num = new QLabel(ptr);
        label_num->setNum(i);

        label_num->setMinimumSize(QSize(15, 25));
        label_num->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label_num->setStyleSheet(QString::fromUtf8("background: rgb(141, 192, 255)"));

        horizontalLayout->addWidget(label_num);

        auto label_fpn = new QLabel(ptr);
        label_fpn->setText("Нет данных");

        horizontalLayout->addWidget(label_fpn);

        ui->verticalLayout_76->addWidget(ptr);
    }

    curr_editor_field["freq_plans"] = 0;

    fps_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Просмотр");
    fps_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить");
    fps_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить");

    for(int i = 0; i < 3; i++){
        ui->fps_popup_menu_list->addItem(fps_popup_menu_list_item[i]);
    }
    ui->fps_popup_menu_list->setCurrentItem(fps_popup_menu_list_item[0]);

    fp_popup_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    fp_popup_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Добавить диапазон");
    fp_popup_menu_list_item[2] = new QListWidgetItem(QIcon(""), "Удалить диапазон");
    fp_popup_menu_list_item[3] = new QListWidgetItem(QIcon(""), "Тестировать план");
    fp_popup_menu_list_item[4] = new QListWidgetItem(QIcon(""), "Авто.формирование");

    for(int i = 0; i < 5; i++){
        ui->fp_popup_menu_list->addItem(fp_popup_menu_list_item[i]);
    }
    ui->fp_popup_menu_list->setCurrentItem(fp_popup_menu_list_item[0]);

    freq_editor.append(QList<QWidget*>{
                          ui->widget_81,
                           ui->widget_82,
                           ui->widget_86,
                       });

    for(int i = 1; i <= 32; i++){
        QWidget* ptr;
        keys_list.push_back(ptr = new QWidget(ui->scrollAreaWidgetContents_3));

        //ptr->setMinimumSize(QSize(0, 25));

        auto horizontalLayout = new QHBoxLayout(ptr);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        auto label_num = new QLabel(ptr);
        label_num->setNum(i);

        label_num->setMinimumSize(QSize(15, 25));
        label_num->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        label_num->setStyleSheet(QString::fromUtf8("background: rgb(141, 192, 255)"));

        horizontalLayout->addWidget(label_num);

        auto label_fpn = new QLabel(ptr);
        label_fpn->setText("Нет данных");

        horizontalLayout->addWidget(label_fpn);

        ui->verticalLayout_89->addWidget(ptr);
    }

    curr_editor_field["keys_list"] = 0;


    keys_list_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Редактировать");
    keys_list_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Удалить");

    for(int i = 0; i < 2; i++){
        ui->keys_list_menu_list->addItem(keys_list_menu_list_item[i]);
    }

    ui->keys_list_menu_list->setCurrentItem(keys_list_menu_list_item[0]);

    key_editor_menu_list_item[0] = new QListWidgetItem(QIcon(""), "Сохранить");
    key_editor_menu_list_item[1] = new QListWidgetItem(QIcon(""), "Генерировать");

    for(int i = 0; i < 2; i++){
        ui->key_editor_menu_list->addItem(key_editor_menu_list_item[i]);
    }

    ui->key_editor_menu_list->setCurrentItem(key_editor_menu_list_item[0]);

    key_editor.append(QList<QWidget*>{
                          ui->widget_97,
                          ui->widget_90,
                          ui->widget_91,
                          ui->widget_92,
                          ui->widget_93,
                          ui->widget_94,
                          ui->widget_95,
                          ui->widget_96
                      });

    update_mask_key_popup_list();
    update_fp_popup_list();

    ui->direction_selection_list->setIconSize(QSize(32, 32));
    ui->direction_list->setIconSize(QSize(32, 32));
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

void MainWindow::show_rules()
{
    ruleswin.show();
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

    if(current_direction != nullptr && current_direction->is_idle){
        ui->main_background->setStyleSheet("background-color: black");
        hide_dej_labels();
        ui->direction_label->setText("");
        ui->channel_label->setText(current_direction->name);
        return;
    }

    if(current_direction != nullptr){
        ui->direction_label->setText(current_direction->name);
        if(current_direction->ch != nullptr){
            ui->channel_label->setText(current_direction->ch->name);
        }
        else{
            ui->channel_label->setText("");
        }

        ui->main_background->setStyleSheet(
                    "#main_background {"
                    " border-image: url(:/resources/back (" + QString::number(current_direction->background) + ").png)"
                                                                                                               "}");

        show_dej_labels();
    }
    else{
        ui->main_background->setStyleSheet("background-color: black");
        hide_dej_labels();
        ui->direction_label->setText("");
        ui->channel_label->setText("");
    }
}

void MainWindow::menu_screen()
{
    ui->mainPages->setCurrentWidget(ui->menu_page);
    ui->modals->setCurrentWidget(ui->no_modals);
    if(current_direction != nullptr){
        show_dej_labels();
    }
    else{
        hide_dej_labels();
    }
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
        if(current_direction != nullptr && current_direction->ch != nullptr && !current_direction->is_idle){
            ui->label_94->setText(current_direction->ch->name);
            ui->atuners->setCurrentWidget(ui->noise);
            ui->noise->setProperty("from_menu", true);
            noise_show();
            return;
        }
    }
    if(item == menu_list_item[1]){
        main_screen();
        ui->atuners->setCurrentWidget(ui->volume);
        ui->volume->setProperty("from_menu", true);
        volume_show();
        return;
    }
    if(item == menu_list_item[2]){
        navigation_screen();
        return;
    }
    if(item == menu_list_item[3]){
        received_messages_screen();
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

void MainWindow::on_ctcss_popup_itemSelectionChanged()
{
    selected_items["channel_editor_ctcss_popup"] = ui->ctcss_popup->currentItem();
}

void MainWindow::on_scan_popup_itemSelectionChanged()
{
    selected_items["direction_editor_scan_popup"] = ui->scan_popup->currentItem();
}


void MainWindow::on_direction_list_itemSelectionChanged()
{
    selected_items["direction_list"] = ui->direction_list->currentItem();
}

void MainWindow::on_channel_choice_list_itemSelectionChanged()
{
    selected_items["channel_choice_list"] = ui->channel_choice_list->currentItem();
}

void MainWindow::on_nav_menu_list_itemSelectionChanged()
{
    selected_items["nav_menu_list"] = ui->nav_menu_list->currentItem();
}

void MainWindow::on_fps_popup_menu_list_itemSelectionChanged()
{
    selected_items["fps_popup_menu_list"] = ui->fps_popup_menu_list->currentItem();
}

void MainWindow::on_fp_popup_menu_list_itemSelectionChanged()
{
    selected_items["fp_popup_menu_list"] = ui->fp_popup_menu_list->currentItem();
}

void MainWindow::on_keys_list_menu_list_itemSelectionChanged()
{
    selected_items["keys_list_menu_list"] = ui->keys_list_menu_list->currentItem();
}

void MainWindow::on_key_editor_menu_list_itemSelectionChanged()
{
    selected_items["key_editor_menu_list"] = ui->key_editor_menu_list->currentItem();
}

void MainWindow::on_mask_key_popup_list_itemSelectionChanged()
{
    selected_items["mask_key_popup_list"] = ui->mask_key_popup_list->currentItem();
}

void MainWindow::on_fp_popup_list_itemSelectionChanged()
{
    selected_items["fp_popup_list"] = ui->fp_popup_list->currentItem();
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

void MainWindow::navigation_screen(){
    ui->mainPages->setCurrentWidget(ui->navigation_page);
}

void MainWindow::received_messages_screen(){
    ui->mainPages->setCurrentWidget(ui->received_messages_page);
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

void MainWindow::freq_plans_screen(){
    ui->mainPages->setCurrentWidget(ui->freq_plans_page);
    update_freq_plans_screen();
}

void MainWindow::keys_list_screen(){
    ui->mainPages->setCurrentWidget(ui->keys_list_page);

    for(int i = 0; i < 32; i++){
        if(keys_vec[i] != nullptr){
            dynamic_cast<QLabel*>(keys_list[i]->children()[2])->setText(QString("Ключ (CRC: %1)").arg(keys_vec[i]->crc));
        }
    }

    update_keys_list_screen();
}

void MainWindow::freq_plan_screen(){
    ui->mainPages->setCurrentWidget(ui->freq_plan_page);
    ui->listWidget->clear();
    for(auto r : freq_plan_vec[curr_editor_field["freq_plans"]]->ranges){
        ui->listWidget->addItem(new QListWidgetItem(r->info()));
    }
    ui->label_169->setVisible(freq_plan_vec[curr_editor_field["freq_plans"]]->ranges.empty());

    ui->label_164->setText(QString("%1 Частотный план").arg(curr_editor_field["freq_plans"] + 1));
    ui->label_163->setText(freq_plan_vec[curr_editor_field["freq_plans"]]->ranges.empty() ? QString("Список диапазонов") : QString("Список диапазонов [%1]").arg(freq_plan_vec[curr_editor_field["freq_plans"]]->ranges.size()));
}

void MainWindow::freq_editor_screen(){
    ui->mainPages->setCurrentWidget(ui->freq_editor_page);
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    update_freq_editor_screen();
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
    if(selected_items["direction_list"] == nullptr){
        ui->empty_direction_list_label->setVisible(true);
    }
    else{
        ui->empty_direction_list_label->setVisible(false);
    }
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
        return;
    }
    if(item == data_editor_list_item[2]){
        channel_list_screen();
        return;
    }
    if(item == data_editor_list_item[3]){
        //scan lists
        return;
    }
    if(item == data_editor_list_item[4]){
        freq_plans_screen();
        return;
    }
    if(item == data_editor_list_item[5]){
        keys_list_screen();
        return;
    }
    if(item == data_editor_list_item[6]){

        return;
    }
    if(item == data_editor_list_item[7]){

        return;
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
    ui->modals->setCurrentWidget(ui->wait);
    delay(1000);
    ui->modals->setCurrentWidget(ui->no_modals);
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
    if(ui->modals->currentWidget() == ui->channel_popup_menu){
        ui->channel_list_left->setText("Меню");
        ui->modals->setCurrentWidget(ui->no_modals);
    }
    else{
        data_editor_screen();
    }
}

void MainWindow::on_direction_list_right_clicked()
{
    if(ui->modals->currentWidget() == ui->direction_popup_menu){
        ui->direction_list_left->setText("Меню");
        ui->modals->setCurrentWidget(ui->no_modals);
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
    if(ui->modals->currentWidget() == ui->channel_popup_menu){
        // menu selection
        on_channel_popup_menu_list_itemDoubleClicked(selected_items["channel_popup_menu_list"]);
    }
    else{
        // menu activation
        ui->modals->setCurrentWidget(ui->channel_popup_menu);
        ui->channel_popup_menu_list->setCurrentItem(selected_items["channel_popup_menu_list"]);
        ui->channel_list_left->setText("Выбрать");
    }
}

bool MainWindow::delete_channel(QListWidgetItem *item){
    if(item == nullptr) return false;

    for(auto d : channel_map[item].channel->used_by){
        d->is_idle = true;
        d->name = "Idle";
        d->ch = new Channel();
        d->ch->name = "Не определен";
    }

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
        ui->modals->setCurrentWidget(ui->no_modals);
        ui->channel_list_left->setText("Меню");
        channel_editor_screen();
    }
    // ADD
    if(item == channel_popup_menu_list_item[1]){
        QListWidgetItem* ref = new QListWidgetItem(QIcon(":/resources/white16.png"), "");
        QListWidgetItem* ref2 = new QListWidgetItem(QIcon(":/resources/white16.png"), "");
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

        ui->modals->setCurrentWidget(ui->no_modals);
        ui->channel_list_left->setText("Меню");
        channel_editor_screen();
    }
    // DELETE
    if(item == channel_popup_menu_list_item[2]){
        if(delete_channel(selected_items["channel_list"])){
            ui->modals->setCurrentWidget(ui->no_modals);
            ui->channel_list_left->setText("Меню");
            update_channel_list_screen();
        }
    }
}

void MainWindow::set_default_channel_fields(){
    // dmo
    ui->dmo_pprch->setChecked(false);
    ui->dmo_retr->setChecked(false);
    ui->dmo_prd_only_retr->setChecked(false);
    ui->dmo_dualfreq->setChecked(false);
    ui->dmo_mcc->setText("250");
    ui->dmo_mnc->setText("1");
    ui->dmo_gssi->setText("");
    ui->dmo_sos->setText("");
    ui->dmo_f_dmo->setText("");
    ui->dmo_f_retr->setText("");
    ui->dmo_tx_dmo->setText("");
    ui->dmo_chp_dmo->setText("Не задано");
    ui->dmo_chp_dmo->setProperty("chosen", 0);
    ui->dmo_chp_retr->setText("Не задано");
    ui->dmo_chp_retr->setProperty("chosen", 0);
    ui->dmo_chp_prd->setText("Не задано");
    ui->dmo_chp_prd->setProperty("chosen", 0);
    ui->dmo_prm_hz->setText("");
    ui->dmo_prd_hz->setText("");
    ui->dmo_prm_net->setText("Не задано");
    ui->dmo_prm_net->setProperty("chosen", 0);
    ui->dmo_prd_net->setText("Не задано");
    ui->dmo_prd_net->setProperty("chosen", 0);
    ui->dmo_n_retr->setText("");
    ui->dmo_mask->setChecked(false);
    ui->dmo_mask_key->setText("Нет");
    ui->dmo_mask_key->setProperty("chosen", 0);
    ui->dmo_name->setText("");

    ui->mask_key_popup_list->setCurrentRow(0);
    ui->fp_popup_list->setCurrentRow(0);

    // tmo
    ui->tmo_mcc->setText("250");
    ui->tmo_mnc->setText("1");
    ui->tmo_gssi->setText("");
    ui->tmo_vesh->setChecked(false);
    ui->tmo_mask->setChecked(false);
    ui->tmo_mask_key->setText("Нет");
    ui->tmo_mask_key->setProperty("chosen", 0);
    ui->mask_key_popup_list->setCurrentRow(0);
    ui->tmo_name->setText("");

    // vpd
    ui->vpd_mcc->setText("250");
    ui->vpd_mnc->setText("1");
    ui->vpd_gssi->setText("");
    ui->vpd_freq->setText("");
    ui->vpd_name->setText("");

    // am25
    ui->am25_prd->setChecked(false);
    ui->am25_dualfreq->setChecked(false);
    ui->am25_freq->setText("");
    ui->am25_prm_freq->setText("");
    ui->am25_prd_freq->setText("");
    ui->am25_name->setText("");

    // chm25
    ui->chm25_prd->setChecked(false);
    ui->chm25_dualfreq->setChecked(false);
    ui->chm25_freq->setText("");
    ui->chm25_prm_freq->setText("");
    ui->chm25_prd_freq->setText("");
    ui->ctcss_popup->setCurrentRow(0);
    ui->chm25_ctcss->setText(ui->ctcss_popup->currentItem()->text());
    ui->chm25_name->setText("");

    // chm50
    ui->chm50_prd->setChecked(false);
    ui->chm50_dualfreq->setChecked(false);
    ui->chm50_freq->setText("");
    ui->chm50_prm_freq->setText("");
    ui->chm50_prd_freq->setText("");
    ui->ctcss_popup->setCurrentRow(0);
    ui->chm50_ctcss->setText(ui->ctcss_popup->currentItem()->text());
    ui->chm50_name->setText("");

    // obp
    ui->obp_prd->setChecked(false);
    ui->obp_band->setProperty("band", 1);
    ui->obp_band->setText("Верхняя");
    ui->obp_freq->setText("");
    ui->obp_name->setText("");

    // fm
    ui->fm_prd->setChecked(false);
    ui->fm_dualfreq->setChecked(false);
    ui->fm_freq->setText("");
    ui->fm_prm_freq->setText("");
    ui->fm_prd_freq->setText("");
    ui->fm_name->setText("");
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
        ui->dmo_pprch->setChecked(curr->pprch);
        ui->dmo_retr->setChecked(curr->retr);
        ui->dmo_prd_only_retr->setChecked(curr->prd_only_retr);
        ui->dmo_dualfreq->setChecked(curr->dualfreq);
        ui->dmo_mcc->setText(curr->mcc);
        ui->dmo_mnc->setText(curr->mnc);
        ui->dmo_gssi->setText(curr->gssi);
        ui->dmo_sos->setText(curr->sos);
        ui->dmo_f_dmo->setText(curr->f_dmo);
        ui->dmo_f_retr->setText(curr->f_retr);
        ui->dmo_tx_dmo->setText(curr->tx_dmo);
        ui->dmo_chp_dmo->setProperty("chosen", curr->chp_dmo);
        ui->dmo_chp_dmo->setText(curr->chp_dmo == 0 ? QString("Не задано") : QString("%1 ЧП").arg(curr->chp_dmo));
        ui->dmo_chp_retr->setProperty("chosen", curr->chp_retr);
        ui->dmo_chp_retr->setText(curr->chp_retr == 0 ? QString("Не задано") : QString("%1 ЧП").arg(curr->chp_retr));
        ui->dmo_chp_prd->setProperty("chosen", curr->chp_prd);
        ui->dmo_chp_prd->setText(curr->chp_prd == 0 ? QString("Не задано") : QString("%1 ЧП").arg(curr->chp_prd));
        ui->dmo_prm_hz->setText(curr->prm_hz);
        ui->dmo_prd_hz->setText(curr->prd_hz);
        ui->dmo_prm_net->setProperty("chosen", curr->prm_net);
        ui->dmo_prm_net->setText(curr->prm_net == 0 ? QString("Не задано") : QString("%1 ЧП").arg(curr->prm_net));
        ui->dmo_prd_net->setProperty("chosen", curr->prd_net);
        ui->dmo_prd_net->setText(curr->prd_net == 0 ? QString("Не задано") : QString("%1 ЧП").arg(curr->prd_net));
        ui->dmo_n_retr->setText(curr->n_retr);
        ui->dmo_mask->setChecked(curr->mask);
        ui->dmo_mask_key->setProperty("chosen", curr->mask_key);
        ui->dmo_mask_key->setText(curr->mask_key == 0 ? QString("Нет") : QString("%1 Ключ").arg(curr->mask_key));
        ui->dmo_name->setText(curr->name);

        ui->mask_key_popup_list->setCurrentRow(0); // вообще немного другое должно быть, но мне лень опять в цикле искать
        ui->fp_popup_list->setCurrentRow(0); // вообще немного другое должно быть, но мне лень опять в цикле искать

        break;
    }
    case 2: // tmo
    {
        ui->tmo_mcc->setText(curr->mcc);
        ui->tmo_mnc->setText(curr->mnc);
        ui->tmo_gssi->setText(curr->gssi);
        ui->tmo_vesh->setChecked(curr->vesh);
        ui->tmo_mask->setChecked(curr->mask);
        ui->tmo_name->setText(curr->name);
        ui->tmo_mask_key->setProperty("chosen", curr->mask_key);
        ui->tmo_mask_key->setText(curr->mask_key == 0 ? QString("Нет") : QString("%1 Ключ").arg(curr->mask_key));
        ui->mask_key_popup_list->setCurrentRow(0); // вообще немного другое должно быть, но мне лень опять в цикле искать
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
    {
        ui->fm_prd->setChecked(curr->PRD);
        ui->fm_dualfreq->setChecked(curr->dualfreq);
        ui->fm_freq->setText(QString::number(curr->freq));
        ui->fm_prm_freq->setText(QString::number(curr->prm_freq));
        ui->fm_prd_freq->setText(QString::number(curr->prd_freq));
        ui->fm_name->setText(curr->name);
        break;
    }
    }

    // Always point to the first field first
    curr_editor_field[channel_types[curr->state]] = 0;

    update_channel_editor_page();
}

template<typename T>
int n_not_0(std::vector<T*> v, int n){
    for(int i = 0; i < 32; i++){
        if(v[i]){
            if(n-- == 0){
                return i;
            }
        }
    }
    return 0;
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

    if(ui->channel_editor_state->property("chosen") == 1){
        switch (curr_editor_field["dmo"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->dmo_pprch->toggle();
            break;
        case 2:
            ui->dmo_retr->toggle();
            break;
        case 3:
            ui->dmo_prd_only_retr->toggle();
            break;
        case 4:
            ui->dmo_dualfreq->toggle();
            break;
        case 5:
            ui->dmo_mcc->backspace();
            break;
        case 6:
            ui->dmo_mnc->backspace();
            break;
        case 7:
            ui->dmo_gssi->backspace();
            break;
        case 8:
            ui->dmo_sos->backspace();
            break;
        case 9:
            ui->dmo_f_dmo->backspace();
            break;
        case 10:
            ui->dmo_f_retr->backspace();
            break;
        case 11:
            ui->dmo_tx_dmo->backspace();
            break;
        case 12:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->fp_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 13:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->fp_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 14:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->fp_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 15:
            ui->dmo_prm_hz->backspace();
            break;
        case 16:
            ui->dmo_prd_hz->backspace();
            break;
        case 17:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->fp_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 18:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->fp_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 19:
            ui->dmo_n_retr->backspace();
            break;
        case 20:
            ui->dmo_mask->toggle();
            break;
        case 21:
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->mask_key_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 22:
            ui->dmo_name->backspace();
            break;
        default:
            qCritical("crit: on_channel_editor_right_clicked");
            return;
        }
        update_channel_editor_page();
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
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->modals->setCurrentWidget(ui->mask_key_popup);
            }
            else{
                ui->modals->setCurrentWidget(ui->no_modals);
            }
            break;
        case 8:
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
                set_default_channel_fields();
            }
            ui->channel_editor_state->setProperty("chosen", ui->ch_type_popup->currentRow());
            ui->modals->setCurrentWidget(ui->no_modals);
            update_channel_editor_page();
            return;
        }
    }

    if(ui->channel_editor_state->property("chosen") == 1){
        if(ui->modals->currentWidget() == ui->mask_key_popup){
            int t = ui->mask_key_popup_list->currentRow();
            ui->dmo_mask_key->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(keys_vec, t - 1));
            if(t == 0){
                ui->dmo_mask_key->setText("Нет");
            }
            else{
                ui->dmo_mask_key->setText(QString::number(ui->dmo_mask_key->property("chosen").toInt()));
            }
            ui->modals->setCurrentWidget(ui->no_modals);
            update_channel_editor_page();
            return;
        }
        if(ui->modals->currentWidget() == ui->fp_popup){
            int t = ui->fp_popup_list->currentRow();
            switch (curr_editor_field["dmo"]) {
            case 12:
                ui->dmo_chp_dmo->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(freq_plan_vec, t - 1));
                if(t == 0){
                    ui->dmo_chp_dmo->setText("Не задано");
                }
                else{
                    ui->dmo_chp_dmo->setText(QString("%1 ЧП").arg(ui->dmo_chp_dmo->property("chosen").toInt()));
                }
                break;
            case 13:
                ui->dmo_chp_retr->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(freq_plan_vec, t - 1));
                if(t == 0){
                    ui->dmo_chp_retr->setText("Не задано");
                }
                else{
                    ui->dmo_chp_retr->setText(QString("%1 ЧП").arg(ui->dmo_chp_retr->property("chosen").toInt()));
                }
                break;
            case 14:
                ui->dmo_chp_prd->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(freq_plan_vec, t - 1));
                if(t == 0){
                    ui->dmo_chp_prd->setText("Не задано");
                }
                else{
                    ui->dmo_chp_prd->setText(QString("%1 ЧП").arg(ui->dmo_chp_prd->property("chosen").toInt()));
                }
                break;
            case 17:
                ui->dmo_prm_net->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(freq_plan_vec, t - 1));
                if(t == 0){
                    ui->dmo_prm_net->setText("Не задано");
                }
                else{
                    ui->dmo_prm_net->setText(QString("%1 ЧП").arg(ui->dmo_prm_net->property("chosen").toInt()));
                }
                break;
            case 18:
                ui->dmo_prd_net->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(freq_plan_vec, t - 1));
                if(t == 0){
                    ui->dmo_prd_net->setText("Не задано");
                }
                else{
                    ui->dmo_prd_net->setText(QString("%1 ЧП").arg(ui->dmo_prd_net->property("chosen").toInt()));
                }
                break;
            default:
                qDebug() << "crit: ui->modals->currentWidget() == ui->fp_popup";
                break;
            }
            ui->modals->setCurrentWidget(ui->no_modals);
            update_channel_editor_page();
            return;
        }
    }

    if(ui->channel_editor_state->property("chosen") == 2){
        if(ui->modals->currentWidget() == ui->mask_key_popup){
            int t = ui->mask_key_popup_list->currentRow();
            ui->tmo_mask_key->setProperty("chosen", t == 0 ? 0 : 1 + n_not_0(keys_vec, t - 1));
            if(t == 0){
                ui->tmo_mask_key->setText("Нет");
            }
            else{
                ui->tmo_mask_key->setText(QString::number(ui->tmo_mask_key->property("chosen").toInt()));
            }
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
    case 1: // dmo
    {
        u32 mcc = ui->dmo_mcc->text().toInt();
        u32 mnc = ui->dmo_mnc->text().toInt();
        u32 gssi = ui->dmo_gssi->text().toInt();
        if (!in_range(mcc, 0, 1000) || !in_range(mnc, 0, 1000) || !in_range(gssi, 0, 16777216))
        {
            ERR
        }
        break;
    }
    case 2: // tmo
    {
        u32 mcc = ui->tmo_mcc->text().toInt();
        u32 mnc = ui->tmo_mnc->text().toInt();
        u32 gssi = ui->tmo_gssi->text().toInt();
        if (!in_range(mcc, 0, 1000) || !in_range(mnc, 0, 1000) || !in_range(gssi, 0, 16777216))
        {
            ERR
        }
        /*
        if (ui->tmo_name->text().isEmpty())
        {
            ERR
        }
*/
        break;
    }
    case 3: // vpd
    {
        left = 1000000;
        right = 1000000000;
        u32 mcc = ui->vpd_mcc->text().toInt();
        u32 mnc = ui->vpd_mnc->text().toInt();
        u32 gssi = ui->vpd_gssi->text().toInt();
        if (!in_range(mcc, 0, 1000) || !in_range(mnc, 0, 1000) || !in_range(gssi, 0, 16777216))
        {
            ERR
        }
        u32 freq = ui->vpd_freq->text().toInt();
        if (!in_range(freq, left, right))
        {
            ERR
        }
        /*
        if (ui->vpd_name->text().isEmpty())
        {
            ERR
        }
*/
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
        /*
        if (ui->am25_name->text().isEmpty())
        {
            ERR
        }
*/
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
        /*
        if (ui->chm25_name->text().isEmpty())
        {
            ERR
        }
*/
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
        /*
        if (ui->chm25_name->text().isEmpty())
        {
            ERR
        }
*/
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
        /*
        if (ui->obp_name->text().isEmpty())
        {
            ERR
        }
*/
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
        /*
        if (ui->fm_name->text().isEmpty())
        {
            ERR
        }
*/
        break;
    }
    }

    // clearing before saving
    curr->clear();

    curr->state = state;

    switch (curr->state)
    {
    case 0: // none
        curr->name = tr("Не задано 0.000");
        break;
    case 1: // dmo
    {
        curr->mode = Mode::TETRA_DMO;
        curr->pprch = ui->dmo_pprch->isChecked();
        curr->retr = ui->dmo_retr->isChecked();
        curr->prd_only_retr = ui->dmo_prd_only_retr->isChecked();
        curr->dualfreq = ui->dmo_dualfreq->isChecked();
        curr->mcc = ui->dmo_mcc->text().trimmed();
        curr->mnc = ui->dmo_mnc->text().trimmed();
        curr->gssi = ui->dmo_gssi->text().trimmed();
        curr->sos = ui->dmo_sos->text().trimmed();
        curr->f_dmo = ui->dmo_f_dmo->text().trimmed();
        curr->f_retr = ui->dmo_f_retr->text().trimmed();
        curr->tx_dmo = ui->dmo_tx_dmo->text().trimmed();
        curr->chp_dmo = ui->dmo_chp_dmo->property("chosen").toInt();
        curr->chp_retr = ui->dmo_chp_retr->property("chosen").toInt();
        curr->chp_prd = ui->dmo_chp_prd->property("chosen").toInt();
        curr->prm_hz = ui->dmo_prm_hz->text().trimmed();
        curr->prd_hz = ui->dmo_prd_hz->text().trimmed();
        curr->prm_net = ui->dmo_prm_net->property("chosen").toInt();
        curr->prd_net = ui->dmo_prd_net->property("chosen").toInt();
        curr->n_retr = ui->dmo_n_retr->text().trimmed();
        curr->mask = ui->dmo_mask->isChecked();
        curr->mask_key = ui->dmo_mask_key->property("chosen").toInt();

        if(ui->dmo_name->text().isEmpty()){
            curr->name = "DMO N" + curr->gssi;
        }else{
            curr->name = ui->dmo_name->text();
        }
        break;
    }
    case 2: // tmo
    {
        curr->mode = Mode::TETRA_TMO;
        curr->mcc = ui->tmo_mcc->text().trimmed();
        curr->mnc = ui->tmo_mnc->text().trimmed();
        curr->gssi = ui->tmo_gssi->text().trimmed();
        curr->vesh = ui->tmo_vesh->isChecked();
        curr->mask = ui->tmo_mask->isChecked();
        curr->mask_key = ui->tmo_mask_key->property("chosen").toInt();
        if(ui->tmo_name->text().isEmpty()){
            curr->name = "TMO N" + curr->gssi;
        }else{
            curr->name = ui->tmo_name->text();
        }
        break;
    }
    case 3:  // vpd
    {
        curr->mode = Mode::VPD;
        curr->mcc = ui->vpd_mcc->text().trimmed();
        curr->mnc = ui->vpd_mnc->text().trimmed();
        curr->gssi = ui->vpd_gssi->text().trimmed();
        curr->freq = ui->vpd_freq->text().toInt();
        if(ui->vpd_name->text().isEmpty()){
            curr->name = "ВПД " + getFormatFreq(curr->freq);
        }else{
            curr->name = ui->vpd_name->text();
        }
        break;
    }
    case 4: // am25
    {
        curr->mode = Mode::AM25;
        curr->PRD = ui->am25_prd->isChecked();
        curr->dualfreq = ui->am25_dualfreq->isChecked();
        curr->freq = ui->am25_freq->text().toInt();
        curr->prm_freq = ui->am25_prm_freq->text().toInt();
        curr->prd_freq = ui->am25_prd_freq->text().toInt();
        if(ui->am25_name->text().isEmpty()){
            curr->name = "АМ25 " + getFormatFreq(curr->dualfreq ? curr->prd_freq : curr->freq);
        }else{
            curr->name = ui->am25_name->text();
        }
        break;
    }
    case 5: // chm25
    {
        curr->mode = Mode::CHM25;
        curr->PRD = ui->chm25_prd->isChecked();
        curr->dualfreq = ui->chm25_dualfreq->isChecked();
        curr->freq = ui->chm25_freq->text().toInt();
        curr->prm_freq = ui->chm25_prm_freq->text().toInt();
        curr->prd_freq = ui->chm25_prd_freq->text().toInt();
        curr->ctcss = ui->chm25_ctcss->property("chosen").toInt();
        if(ui->chm25_name->text().isEmpty()){
            curr->name = "ЧМ25 " + getFormatFreq(curr->dualfreq ? curr->prd_freq : curr->freq);
        }else{
            curr->name = ui->chm25_name->text();
        }
        break;
    }
    case 6: // chm50
    {
        curr->mode = Mode::CHM50;
        curr->PRD = ui->chm50_prd->isChecked();
        curr->dualfreq = ui->chm50_dualfreq->isChecked();
        curr->freq = ui->chm50_freq->text().toInt();
        curr->prm_freq = ui->chm50_prm_freq->text().toInt();
        curr->prd_freq = ui->chm50_prd_freq->text().toInt();
        curr->ctcss = ui->chm50_ctcss->property("chosen").toInt();
        if(ui->chm50_name->text().isEmpty()){
            curr->name = "ЧМ50 " + getFormatFreq(curr->dualfreq ? curr->prd_freq : curr->freq);
        }else{
            curr->name = ui->chm50_name->text();
        }
        break;
    }
    case 7: // obp
    {
        curr->mode = Mode::OBP;
        curr->PRD = ui->obp_prd->isChecked();
        curr->band = ui->obp_band->property("band") == 1 ? 1 : 0;
        curr->freq = ui->obp_freq->text().toInt();
        if(ui->obp_name->text().isEmpty()){
            curr->name = "ОБП " + getFormatFreq(curr->freq);
        }else{
            curr->name = ui->obp_name->text();
        }
        break;
    }
    case 8: // fm
    {
        curr->mode = Mode::FM;
        curr->PRD = ui->fm_prd->isChecked();
        curr->dualfreq = ui->fm_dualfreq->isChecked();
        curr->freq = ui->fm_freq->text().toInt();
        curr->prm_freq = ui->fm_prm_freq->text().toInt();
        curr->prd_freq = ui->fm_prd_freq->text().toInt();
        if(ui->fm_name->text().isEmpty()){
            curr->name = "FM " + getFormatFreq(curr->dualfreq ? curr->prd_freq : curr->freq);
        }else{
            curr->name = ui->fm_name->text();
        }
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
    if(ui->modals->currentWidget() == ui->direction_popup_menu){
        // menu selection
        on_direction_popup_menu_list_itemDoubleClicked(selected_items["direction_popup_menu_list"]);
    }
    else{
        // menu activation
        ui->modals->setCurrentWidget(ui->direction_popup_menu);
        ui->direction_popup_menu_list->setCurrentItem(selected_items["direction_popup_menu_list"]);
        ui->direction_list_left->setText("Выбрать");
    }
}

void MainWindow::delete_direction(QListWidgetItem *item){
    if(selected_items["direction_list"] != nullptr){
        if(direction_map[item].direction->ch){
            direction_map[item].direction->ch->used_by.erase(direction_map[item].direction);
            if(direction_map[item].direction->ch->used_by.empty()){
                auto ch = direction_map[item].direction->ch;
                for(auto kv : channel_map){
                    if(kv.second.channel == ch){
                        kv.first->setIcon(QIcon(":/resources/white16.png"));
                        kv.second.ref2->setIcon(QIcon(":/resources/white16.png"));
                        break;
                    }
                }
            }
        }

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

        ui->modals->setCurrentWidget(ui->no_modals);
        ui->direction_list_left->setText("Меню");
        //TODO ~update_direction_list
    }
}

void MainWindow::on_direction_popup_menu_list_itemDoubleClicked(QListWidgetItem *item)
{
    //look at the choice
    // EDIT
    if(item == direction_popup_menu_list_item[0]){
        if(selected_items["direction_list"] == nullptr) return;
        ui->modals->setCurrentWidget(ui->no_modals);
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

        ui->modals->setCurrentWidget(ui->no_modals);
        ui->direction_list_left->setText("Меню");

        chosen_ref_d = 0;

        direction_editor_screen();
    }
    // DELETE
    if(item == direction_popup_menu_list_item[2]){
        delete_direction(selected_items["direction_list"]);
    }
}

void MainWindow::set_default_direction_fields(){
    ui->is_forbidden_prd_d->setCheckState(Qt::Unchecked);
    ui->is_tone_call->setCheckState(Qt::Unchecked);

    ui->scan->setProperty("chosen", 0);
    ui->scan_popup->setCurrentRow(0);
    ui->scan->setText(ui->scan_popup->currentItem()->text());

    ui->economizer->setProperty("num", 3);
    ui->economizer->setNum(3);
    _on_economizer_numChanged();

    ui->name_d->setText(getNewDirectionName());
    ui->background_dir_picture->setProperty("num", 0);
    ui->background_dir_picture->setNum(ui->background_dir_picture->property("num").toInt());
    _on_direction_background_numChanged();

    ui->priority_d->setText("Нет");
    ui->priority_d->setProperty("chosen", 0);

    ui->timeout_d->setText("0");
    ui->timeout_d->setProperty("chosen", 0);
}

std::string intTo3d(int i){
    std::stringstream ss;
    if(i < 10){
        ss << "00" << i;
        return ss.str();
    }
    if(i < 100){
        ss << "0" << i;
        return ss.str();
    }
    if(i < 1000){
        ss << i;
        return ss.str();
    }
}

QString MainWindow::getFormatFreq(int freq){
    if(freq == 0) return tr("0.000");
    int n =0;
    int freq2 = freq;
    while(freq % 10 == 0){
        n++;
        freq /= 10;
    }
    std::stringstream ss;
    if(n >= 3){
        freq2 /= 1000;
        ss << freq2 / 1000 << '.' << intTo3d(freq2 % 1000);
        return QString(ss.str().c_str());
    }

    if(n == 2){
        freq2 /= 1000;
        ss << freq / 10000 << '.' << intTo3d((freq % 10000) / 10) << '.' << freq % 10;
        return QString(ss.str().c_str());
    }

    if(n == 1){
        ss << freq / 100000 << '.' << intTo3d((freq % 100000) / 100) << '.' << freq % 100;
        return QString(ss.str().c_str());
    }

    if(n == 0){
        ss << freq / 1000000 << '.' << intTo3d((freq % 1000000) / 1000) << '.' << freq % 1000;
        return QString(ss.str().c_str());
    }
}

QString MainWindow::getNewDirectionName(){
    int n = 1;
    std::set<QString> dname_set;
    for(auto kv : direction_map){
        dname_set.insert(kv.second.direction->name);
    }
    QString newname = "Направление ";
    while(dname_set.find(newname + QString::number(n)) != dname_set.end()) n++;
    return newname + QString::number(n);
}

// direction loading
void MainWindow::direction_editor_screen()
{
    ui->mainPages->setCurrentWidget(ui->direction_editor_page);

    set_default_direction_fields();

    Direction* curr = direction_map[selected_items["direction_list"]].direction;

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

    ui->is_forbidden_prd_d->setCheckState(curr->PRD ? Qt::Checked : Qt::Unchecked);
    ui->is_tone_call->setCheckState(curr->tone_call ? Qt::Checked : Qt::Unchecked);
    ui->scan->setProperty("chosen", curr->scan_list);
    ui->scan_popup->setCurrentRow(curr->scan_list);
    ui->scan->setText(ui->scan_popup->currentItem()->text());
    ui->economizer->setProperty("num", curr->economizer);
    ui->economizer->setNum(curr->economizer);
    _on_economizer_numChanged();

    ui->priority_d->setProperty("chosen", curr->priority);
    ui->priority_d->setText(!curr->priority ? "Нет" : QString::number(curr->priority));


    ui->timeout_d->setProperty("chosen", curr->timeout);
    ui->timeout_d->setText(QString::number(curr->timeout));

    //if(curr->name.isEmpty()){
    //    ui->name_d->setText(getNewDirectionName());
    //}
    //else{
        ui->name_d->setText(curr->name);
    //}
    ui->background_dir_picture->setProperty("num", curr->background);
    ui->background_dir_picture->setNum(ui->background_dir_picture->property("num").toInt());
    _on_direction_background_numChanged();


    // Always point to the first field first
    curr_editor_field["dir"] = 0;
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
    if(chosen_ref_d == 0 || curr_editor_field["dir"] == 0){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            if(selected_items["channel_choice_list"] == 0){
                return;
            }
            //Выбрать
            if(channel_map_d[selected_items["channel_choice_list"]]->state == 0) return;
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
        curr->is_idle = true;
        curr->PRD = true;
        selected_items["direction_list"]->setText(curr->name + "\n ");
        selected_items["direction_list"]->setIcon(QIcon(":/resources/black32.png"));
        direction_map[selected_items["direction_list"]].ref2->setText(curr->name);
        direction_map[selected_items["direction_list"]].ref2->setIcon(QIcon(":/resources/black32.png"));
        direction_list_screen();
        return;
    }

    Channel* channel = channel_map_d[chosen_ref_d];
    if(curr_editor_field["dir"] == 4){
        if(ui->modals->currentWidget() == ui->scan_modal){
            //Выбрать
            ui->scan->setText(selected_items["direction_editor_scan_popup"]->text());
            ui->scan->setProperty("chosen",ui->scan_popup->currentRow());
            ui->modals->setCurrentWidget(ui->no_modals);
            update_direction_editor_page();
            return;
        }
    }

    // direction saving
    Direction* curr = direction_map[selected_items["direction_list"]].direction;
    if(curr->ch){
        curr->ch->used_by.erase(curr);
        if(curr->ch->used_by.empty()){
            for(auto kv : channel_map){
                if(kv.second.channel == curr->ch){
                    kv.first->setIcon(QIcon(":/resources/white16.png"));
                    kv.second.ref2->setIcon(QIcon(":/resources/white16.png"));
                    break;
                }
            }
        }
    }
    // clearing before saving
    curr->clear();
    // REFACTOR
    curr->ch = channel; //channel_map_d[selected_items["channel_choice_list"]];
    curr->ch->used_by.insert(curr);

    for(auto kv : channel_map){
        if(kv.second.channel == curr->ch){
            kv.first->setIcon(QIcon(":/resources/yellow_star.png"));
            kv.second.ref2->setIcon(QIcon(":/resources/yellow_star.png"));
            break;
        }
    }

    curr->PRD = ui->is_forbidden_prd_d->isChecked();
    curr->tone_call = ui->is_tone_call->isChecked();
    curr->scan_list = ui->scan->property("chosen").toInt();
    curr->economizer = ui->economizer->property("num").toInt();
    curr->priority = ui->priority_d->property("chosen").toInt();
    curr->timeout = ui->timeout_d->property("chosen").toInt();
    curr->name = ui->name_d->text();
    curr->background = ui->background_dir_picture->property("num").toInt();

    selected_items["direction_list"]->setText(curr->name + "\n" + curr->ch->name);
    selected_items["direction_list"]->setIcon(QIcon(":/resources/back (" + QString::number(curr->background) + ").png"));
    direction_map[selected_items["direction_list"]].ref2->setText(curr->name); // + "\n" + curr->ch->name
    direction_map[selected_items["direction_list"]].ref2->setIcon(QIcon(":/resources/back (" + QString::number(curr->background) + ").png"));

    chosen_ref_d = 0;

    direction_list_screen();
}

void MainWindow::on_direction_editor_right_clicked()
{
    if(chosen_ref_d == 0 || curr_editor_field["dir"] == 0){
        if(ui->direction_editor_stackedWidget->currentWidget() == ui->channel_choice_page){
            //Назад
            if(chosen_ref_d) ui->channel_choice_list->setCurrentItem(chosen_ref_d);
            swap_direction_page();
            update_direction_editor_page();
            return;
        }
        else{
            if(direction_map[selected_items["direction_list"]].direction->is_idle) return;
            //Выбрать
            ui->direction_editor_stackedWidget->setCurrentWidget(ui->channel_choice_page);            
            update_direction_editor_page();
            return;
        }
    }

    if(channel_map_d[chosen_ref_d]->state == 1){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 3:
            // ui->priority_d
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 5:
            ui->timeout_d->setProperty("chosen", (ui->timeout_d->property("num").toInt() + 1) % 16);
            ui->timeout_d->setNum(ui->timeout_d->property("num").toInt());
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 2){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 3:
            // ui->priority_d
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 5:
            ui->timeout_d->setProperty("chosen", (ui->timeout_d->property("num").toInt() + 1) % 16);
            ui->timeout_d->setNum(ui->timeout_d->property("num").toInt());
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 3){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 4){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 6:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 5){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 6:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 6){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 6:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 7){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 6:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    if(channel_map_d[chosen_ref_d]->state == 8){
        switch (curr_editor_field["dir"]) {
        case 0:
            // skip
            break;
        case 1:
            ui->is_forbidden_prd_d->toggle();
            break;
        case 2:
            ui->is_tone_call->toggle();
            break;
        case 4:
            if(ui->modals->currentWidget() == ui->scan_modal){
               ui->modals->setCurrentWidget(ui->no_modals);
            }
            else{
               ui->modals->setCurrentWidget(ui->scan_modal);
            }
            break;
        case 6:
            ui->economizer->setProperty("num", (ui->economizer->property("num").toInt() + 1) % 4);
            ui->economizer->setNum(ui->economizer->property("num").toInt());
            _on_economizer_numChanged();
            break;
        case 7:
            ui->name_d->backspace();
            break;
        case 8:
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

    update_direction_editor_page();
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
    if(selected_items["direction_selection_list"] != nullptr)
    {
        current_direction = direction_map_d[selected_items["direction_selection_list"]];
        set_header();
        memcpy(_buf, &self, sizeof(Header));
        //broadcast_init();
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
    connect(&udpSocket, &QUdpSocket::readyRead, this, &MainWindow::recieveDatagrams);

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

    //connect(&udpSocket, &QIODevice::bytesWritten, this, &MainWindow::recieveDatagrams, Qt::QueuedConnection);
}

void MainWindow::hide_dej_labels(){
    ui->label_dej_1->setText("");
    ui->label_dej_2->setText("");
    ui->label_dej_3->setText("");
}

void MainWindow::show_dej_labels(){
    ui->label_dej_1->setText("Дежурный прием");
    ui->label_dej_2->setText("Дежурный прием");
    ui->label_dej_3->setText("Дежурный прием");
}

void MainWindow::hideDej(){
    if(!--receivedPackets){
        ui->modals->setCurrentWidget(ui->no_modals);
        hide_dej_labels();
    }
}

/// 0 - configs match, 1 - partial match, -1 - no match
int MainWindow::_check_mode_params()
{
    Header& s = self;
    Header& o = corr;
    if (s.mode == o.mode) // modes match
    {
        switch (self.mode)
        {
        case Mode::TETRA_DMO:
            return 0; // TODO: proper compare
            break;
        case Mode::TETRA_TMO: // gssi?
            if (s.mcc == o.mcc && s.mnc == o.mnc)
                return 0;
            break;
        case Mode::VPD: // gssi?
            if (s.mcc == o.mcc && s.mnc == o.mnc)
                return 0;
            break;
        case Mode::AM25:
            return 0;
            break;
        case Mode::CHM25:
            if (s.ctcss == o.ctcss)
                return 0;
            break;
        case Mode::CHM50:
            if (s.ctcss == o.ctcss)
                return 0;
            break;
        case Mode::OBP:
            if (s.freq_band == o.freq_band)
                return 0;
            break;
        case Mode::FM:
            return 0;
            break;
        default:
            break;
        }
    }
    return -1; // modes do not match but we still hear static because freqs match;
}
/// 0 - configs match, 1 - partial match, -1 - no match
int MainWindow::compare_configs()
{
    Header& s = self;
    Header& o = corr;
    if (s.dual_freq) // self is dual freq
    {
        if (o.dual_freq) // other is dual freq
        {
            if (s.recv == o.send) // dual freqs match
                return _check_mode_params();
            else
                return -1;
        }
        else // other is single freq
        {
            if (s.recv == o.freq) // self recv freq matches other single freq
                return _check_mode_params();
            else
                return -1;
        }
    }
    else // self is single freq
    {
        if (o.dual_freq) // other is dual freq
        {
            if (s.freq == o.send) // self single freq matches other send freq
                return _check_mode_params();
            else
                return -1;
        }
        else // other is single freq
        {
            if (s.freq == o.freq)
                return _check_mode_params();
            else
                return -1;
        }
    }
    return -1;
}

void MainWindow::recieveDatagrams()
{    
    qDebug() << "a try to receive a datagrams";
    if (transmitting) {
        qDebug() << "is transmitting -> reject";
        return;
    }

    QByteArray datagram;
    while (udpSocket.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket.pendingDatagramSize()));
        udpSocket.readDatagram(datagram.data(), datagram.size());
    }
    memcpy(&corr, datagram.constData(), sizeof (Header));
//    qDebug() << "self: " << self;
//    qDebug() << "corr: " << corr;

    switch (compare_configs())
    {
    case 0:  // configs match
        setReceiving();
        if(ui->mainPages->currentWidget() == ui->main_page){
            ui->modals->setCurrentWidget(ui->pr_per);
        }
        hide_dej_labels();
        QTimer *t;
        timers.push(t = new QTimer());
        receivedPackets++;
        connect(t, &QTimer::timeout, this, [t, this](){ hideDej(); t->stop(); });
        t->start(1000);
        buffer.append(datagram.constData() + sizeof (Header), datagram.size() - sizeof (Header));
        playSamples();
        break;
     case 1: // partial match
        // TODO: play static
        break;

     case -1: // no match
        break;
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
    datagram.append(_buf, sizeof(_buf));
    datagram.append(inptDev->readAll());
    udpSocket.writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress(ADDR), PORT);
}

void MainWindow::setReceiving(){
    ui->arrow->setStyleSheet("border-image: url(:/resources/pr.png)");

    Channel *ch = current_direction->ch;
    auto t = ch->dualfreq ? getFormatFreq(ch->prd_freq) : getFormatFreq(ch->freq);
    ui->dej_label_1->setText(tr("ПРИЕМ ") + t);
    switch (ch->state) {
    case 1:
        ui->dej_label_2->setText(tr("TETRA_DMO"));
        break;
    case 2:
        ui->dej_label_2->setText(tr("TETRA_TMO"));
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

void MainWindow::setTransmitting(){

    if(current_direction->PRD || current_direction->ch->PRD){
        ui->dej->setStyleSheet("#dej {"
                               "  background:white;"
                               "  image: url(:/resources/red_alert.png);"
                           "}");
        ui->forb_transm_label->setText("ЗАПРЕЩЕНА ПЕРЕДАЧА");
    }
    else{
        ui->dej->setStyleSheet("#dej { background:white; }");
        ui->forb_transm_label->setText("");
    }

    if(current_direction->is_idle){
        ui->dej_label_1->setText("");
        ui->dej_label_2->setText("");
        ui->arrow->setStyleSheet("");
        return;
    }

    ui->arrow->setStyleSheet("border-image: url(:/resources/per.png)");

    Channel *ch = current_direction->ch;
    auto t = ch->dualfreq ? getFormatFreq(ch->prd_freq) : getFormatFreq(ch->freq);
    ui->dej_label_1->setText(tr("ПЕРЕДАЧА ") + t);
    switch (ch->state) {
    case 1:
        ui->dej_label_2->setText(tr("TETRA_DMO"));
        break;
    case 2:
        ui->dej_label_2->setText(tr("TETRA_TMO"));
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

void MainWindow::reset_socket()
{
    udpSocket.close();
    udpSocket.bind(QHostAddress::AnyIPv4, PORT, QUdpSocket::ShareAddress);
    udpSocket.joinMulticastGroup(QHostAddress(ADDR));
    connect(&udpSocket, &QUdpSocket::readyRead, this, &MainWindow::recieveDatagrams);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_F1:
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Адрес сети",
                                             tr("Введите адрес:"), QLineEdit::Normal,
                                             ADDR, &ok);
        if (ok && !text.isEmpty())
        {
            ADDR = text;
            reset_socket();
        }
        break;
    }
    case Qt::Key_F2:
    {
        reset_socket();
        break;
    }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

void MainWindow::on_talk_button_pressed()
{
    if(current_direction != nullptr && current_direction->ch != nullptr)
    {
        if(!transmitting)
        {
            transmitting = true;

            setTransmitting();
            hide_dej_labels();
            if(ui->mainPages->currentWidget() == ui->main_page){                
                ui->modals->setCurrentWidget(ui->pr_per);
            }

            if(current_direction->PRD || current_direction->ch->PRD) return;

            qDebug() << "123";

            inptDev = inpt->start();
            inptConn = connect(inptDev, &QIODevice::readyRead,
                               this, &MainWindow::sendDatagrams, Qt::QueuedConnection);
        }
    }
}

void MainWindow::on_talk_button_released()
{
    if(current_direction != nullptr && current_direction->ch != nullptr)
    {
        transmitting = false;
        ui->modals->setCurrentWidget(ui->no_modals);
        if(!current_direction->is_idle) show_dej_labels();

        if(current_direction->PRD || current_direction->ch->PRD) return;

        inptDev->close();
        inpt->stop();
        disconnect(inptConn);
        reset_socket();

    }
}

void MainWindow::set_header()
{
    if (current_direction == nullptr || current_direction->ch == nullptr)
        return;

    self.clear();
    self.mode = current_direction->ch->mode;
    self.gssi = current_direction->ch->gssi.toInt();
    self.speech_mask = current_direction->ch->mask;
    self.mcc = current_direction->ch->mcc.toInt();
    self.mnc = current_direction->ch->mnc.toInt();
    self.dual_freq = current_direction->ch->dualfreq;
    self.recv = current_direction->ch->prm_freq;
    self.send = current_direction->ch->prd_freq;
    self.freq = current_direction->ch->freq;
    self.freq_band = current_direction->ch->band;
    self.ctcss = current_direction->ch->ctcss;
}

//inline int MainWindow::getFreq(){
//    if(current_direction == nullptr || current_direction->ch == nullptr) return 0;
//    return (int)current_direction->ch->freq;
//}
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
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

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
    if(curr == ui->navigation_page){
        ui->navigation_left->click();
        return;
    }
    if(curr == ui->freq_plans_page){
        ui->freq_plans_left->click();
        return;
    }
    if(curr == ui->freq_plan_page){
        ui->freq_plan_left->click();
        return;
    }
    if(curr == ui->freq_editor_page){
        ui->freq_editor_left->click();
        return;
    }
    if(curr == ui->keys_list_page){
        ui->keys_list_left->click();
    }
    if(curr == ui->key_editor_page){
        ui->key_editor_left->click();
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
    if(ui->modals->currentWidget() == ui->params_error){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }

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
    if(curr == ui->navigation_page){
        ui->navigation_right->click();
        return;
    }
    if(curr == ui->received_messages_page){
        ui->rec_msgs_right->click();
        return;
    }
    if(curr == ui->freq_plans_page){
        ui->freq_plans_right->click();
        return;
    }
    if(curr == ui->freq_plan_page){
        ui->freq_plan_right->click();
        return;
    }
    if(curr == ui->freq_editor_page){
        ui->freq_editor_right->click();
        return;
    }
    if(curr == ui->keys_list_page){
        ui->keys_list_right->click();
    }
    if(curr == ui->key_editor_page){
        ui->key_editor_right->click();
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

        // tetra dmo
        if(ui->channel_editor_state->property("chosen") == 1){
            switch (curr_editor_field["dmo"]) {
            case 5:
                addnle(ui->dmo_mcc, i);
                break;
            case 6:
                addnle(ui->dmo_mnc, i);
                break;
            case 7:
                addnle(ui->dmo_gssi, i);
                break;
            case 8:
                addnle(ui->dmo_sos, i);
                break;
            case 9:
                addnle(ui->dmo_f_dmo, i);
                break;
            case 10:
                addnle(ui->dmo_f_retr, i);
                break;
            case 11:
                addnle(ui->dmo_tx_dmo, i);
                break;
            case 14:
                addnle(ui->dmo_prm_hz, i);
                break;
            case 15:
                addnle(ui->dmo_prd_hz, i);
                break;
            case 19:
                addnle(ui->dmo_n_retr, i);
                break;
            }

            return;
        }

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
    if(curr == ui->freq_editor_page){
        switch (curr_editor_field["freq_editor"]) {
        case 0:
            addnle(ui->lineEdit_2, i);
            break;
        case 1:
            addnle(ui->lineEdit_3, i);
            break;
        }
        return;
    }
    if(curr == ui->key_editor_page){
        addnle(dynamic_cast<QLineEdit*>(key_editor[curr_editor_field["key_editor"]]->children()[2]), i);
    }
}

void go_up(QListWidget* qlw, uint size){
    if(size == 0) return;
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

void MainWindow::clear_direction_fields(){
    ui->channel_in_dir_name->setStyleSheet("background: white;");

    ui->is_forbidden_prd_d->setStyleSheet("background: white;");
    ui->is_tone_call->setStyleSheet("background: white;");
    ui->scan->setStyleSheet("background: white;");
    ui->economizer->setStyleSheet("background: white;");
    ui->priority_d->setStyleSheet("background: white;");
    ui->timeout_d->setStyleSheet("background: white;");

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

void MainWindow::clear_tetra_dmo_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->dmo_pprch->setStyleSheet("background: white;");
    ui->dmo_retr->setStyleSheet("background: white;");
    ui->dmo_prd_only_retr->setStyleSheet("background: white;");
    ui->dmo_dualfreq->setStyleSheet("background: white;");
    ui->dmo_mcc->setStyleSheet("background: white;");
    ui->dmo_mnc->setStyleSheet("background: white;");
    ui->dmo_gssi->setStyleSheet("background: white;");
    ui->dmo_sos->setStyleSheet("background: white;");
    ui->dmo_f_dmo_full->setStyleSheet("background: white;");
    ui->dmo_f_retr_full->setStyleSheet("background: white;");
    ui->dmo_tx_dmo_full->setStyleSheet("background: white;");
    ui->dmo_chp_dmo->setStyleSheet("background: white;");
    ui->dmo_chp_retr->setStyleSheet("background: white;");
    ui->dmo_chp_prd->setStyleSheet("background: white;");
    ui->dmo_prm_hz_full->setStyleSheet("background: white;");
    ui->dmo_prd_hz_full->setStyleSheet("background: white;");
    ui->dmo_prm_net->setStyleSheet("background: white;");
    ui->dmo_prd_net->setStyleSheet("background: white;");
    ui->dmo_n_retr->setStyleSheet("background: white;");
    ui->dmo_mask->setStyleSheet("background: white;");
    ui->dmo_mask_key->setStyleSheet("background: white;");
    ui->dmo_name->setStyleSheet("background: white;");
    ui->label_110->setVisible(false);
    ui->label_112->setVisible(false);
    ui->label_116->setVisible(false);
    ui->label_118->setVisible(false);
    ui->label_128->setVisible(false);
}

void MainWindow::clear_tetra_tmo_fields(){
    ui->channel_editor_state->setStyleSheet("background: white;");
    ui->tmo_net->setStyleSheet("background: white;");
    ui->tmo_mcc->setStyleSheet("background: white;");
    ui->tmo_mnc->setStyleSheet("background: white;");
    ui->tmo_gssi->setStyleSheet("background: white;");
    ui->tmo_mask->setStyleSheet("background: white;");
    ui->tmo_mask_key->setStyleSheet("background: white;");
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
        if(ui->channel_editor_states->currentWidget() != ui->TETRA_DMO_page)
            ui->channel_editor_states->setCurrentWidget(ui->TETRA_DMO_page);
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

    // tetra dmo
    if(ui->channel_editor_state->property("chosen") == 1){
        clear_tetra_dmo_fields();
        switch (curr_editor_field["dmo"]) {
        case 0:
            // was upper
            ui->channel_editor_state->setStyleSheet("border: 2px solid black; background: white;");
            ui->scrollArea->ensureWidgetVisible(ui->widget_60, 0, 10);
            break;
        case 1:
            ui->dmo_pprch->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            ui->scrollArea->ensureWidgetVisible(ui->widget_60, 0, 10);
            break;
        case 2:
            ui->dmo_retr->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            ui->scrollArea->ensureWidgetVisible(ui->widget_54, 0, 10);
            break;
        case 3:
            ui->dmo_prd_only_retr->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            ui->scrollArea->ensureWidgetVisible(ui->widget_55, 0, 10);
            break;
        case 4:
            ui->dmo_dualfreq->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            ui->scrollArea->ensureWidgetVisible(ui->widget_56, 0, 10);
            break;
        case 5:
            ui->dmo_mcc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_43, 0, 10);
            break;
        case 6:
            ui->dmo_mnc->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_44, 0, 10);
            break;
        case 7:
            ui->dmo_gssi->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_42, 0, 10);
            break;
        case 8:
            ui->dmo_sos->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_45, 0, 10);
            break;
        case 9:
            ui->dmo_f_dmo_full->setStyleSheet("#dmo_f_dmo_full {border: 2px solid black; background: white;}");
            ui->label_110->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_39, 0, 10);
            break;
        case 10:
            ui->dmo_f_retr_full->setStyleSheet("#dmo_f_retr_full {border: 2px solid black; background: white;}");
            ui->label_112->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_40, 0, 10);
            break;
        case 11:
            ui->dmo_tx_dmo_full->setStyleSheet("#dmo_tx_dmo_full {border: 2px solid black; background: white;}");
            ui->label_116->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_48, 0, 10);
            break;
        case 12:
            ui->dmo_chp_dmo->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            ui->scrollArea->ensureWidgetVisible(ui->widget_41, 0, 10);
            break;
        case 13:
            ui->dmo_chp_retr->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            ui->scrollArea->ensureWidgetVisible(ui->widget_47, 0, 10);
            break;
        case 14:
            ui->dmo_chp_prd->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            ui->scrollArea->ensureWidgetVisible(ui->widget_59, 0, 10);
            break;
        case 15:
            ui->dmo_prm_hz_full->setStyleSheet("#dmo_prm_hz_full {border: 2px solid black; background: white;}");
            ui->label_118->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_49, 0, 10);
            break;
        case 16:
            ui->dmo_prd_hz_full->setStyleSheet("#dmo_prd_hz_full {border: 2px solid black; background: white;}");
            ui->label_128->setVisible(true);
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_50, 0, 10);
            break;
        case 17:
            ui->dmo_prm_net->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            ui->scrollArea->ensureWidgetVisible(ui->widget_51, 0, 10);
            break;
        case 18:
            ui->dmo_prd_net->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            ui->scrollArea->ensureWidgetVisible(ui->widget_52, 0, 10);
            break;
        case 19:
            ui->dmo_n_retr->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_46, 0, 10);
            break;
        case 20:
            ui->dmo_mask->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Изменить");
            ui->scrollArea->ensureWidgetVisible(ui->widget_57, 0, 10);
            break;
        case 21:
            ui->dmo_mask_key->setStyleSheet("border: 2px solid black; background: white;");

            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }

            ui->scrollArea->ensureWidgetVisible(ui->widget_53, 0, 10);
            break;
        case 22:
            ui->dmo_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->scrollArea->ensureWidgetVisible(ui->widget_58, 0, 10);
            ui->dmo_name->setFocus();
            break;
        default:
            qCritical("dmo: update_channel_editor_page: no way");
        }

        bool pprch = ui->dmo_pprch->isChecked();
        bool retr = ui->dmo_retr->isChecked();
        //bool prd_only_retr = ui->dmo_prd_only_retr->isChecked();
        bool dualfreq = ui->dmo_dualfreq->isChecked();

        if(!pprch && !retr && !dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(true); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(true); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(true);    //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(true); 	//ui->dmo_gssi
            ui->widget_45->setVisible(true); 	//ui->dmo_sos
            ui->widget_39->setVisible(true); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(true); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(false); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(false); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(false); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(false);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(false); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(false); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(false); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(false); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(true); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(true); 	//ui->dmo_mask
            ui->widget_53->setVisible(ui->dmo_mask->isChecked()); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }

        if(pprch && !retr && !dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(true); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(true); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(true);       //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(true); 	//ui->dmo_gssi
            ui->widget_45->setVisible(true); 	//ui->dmo_sos
            ui->widget_39->setVisible(false); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(false); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(true); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(true); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(false);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(false); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(false); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(false); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(false); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(true); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(true); 	//ui->dmo_mask
            ui->widget_53->setVisible(ui->dmo_mask->isChecked()); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }

        if(!pprch && retr && !dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(true); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(false); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(false);       //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(false); 	//ui->dmo_gssi
            ui->widget_45->setVisible(false); 	//ui->dmo_sos
            ui->widget_39->setVisible(false); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(false); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(false); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(false); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(false);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(true); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(true); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(false); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(false); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(true); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(false); 	//ui->dmo_mask
            ui->widget_53->setVisible(false); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }

        if(pprch && retr && !dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(true); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(false); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(false);       //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(false); 	//ui->dmo_gssi
            ui->widget_45->setVisible(false); 	//ui->dmo_sos
            ui->widget_39->setVisible(false); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(false); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(false); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(false); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(false);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(false); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(false); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(true); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(true); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(true); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(false); 	//ui->dmo_mask
            ui->widget_53->setVisible(false); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }

        if(!pprch && !retr && dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(false); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(true);       //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(true); 	//ui->dmo_gssi
            ui->widget_45->setVisible(true); 	//ui->dmo_sos
            ui->widget_39->setVisible(true); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(true); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(false); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(false); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(false);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(false); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(false); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(false); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(false); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(false); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(true); 	//ui->dmo_mask
            ui->widget_53->setVisible(ui->dmo_mask->isChecked()); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }

        if(pprch && !retr && dualfreq){
            ui->widget_60->setVisible(true); 	//ui->dmo_pprch
            ui->widget_54->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_55->setVisible(false); 	//ui->dmo_prd_only_retr
            ui->widget_56->setVisible(true);       //ui->dmo_dualfreq
            ui->widget_43->setVisible(true); 	//ui->dmo_mcc
            ui->widget_44->setVisible(true); 	//ui->dmo_mnc
            ui->widget_42->setVisible(true); 	//ui->dmo_gssi
            ui->widget_45->setVisible(true); 	//ui->dmo_sos
            ui->widget_39->setVisible(false); 	//ui->dmo_f_dmo
            ui->widget_40->setVisible(false); 	//ui->dmo_f_retr
            ui->widget_48->setVisible(false); 	//ui->dmo_tx_dmo
            ui->widget_41->setVisible(true); 	//ui->dmo_chp_dmo
            ui->widget_47->setVisible(false); 	//ui->dmo_chp_retr
            ui->widget_59->setVisible(true);	//ui->dmo_chp_prd
            ui->widget_49->setVisible(false); 	//ui->dmo_prm_hz
            ui->widget_50->setVisible(false); 	//ui->dmo_prd_hz
            ui->widget_51->setVisible(false); 	//ui->dmo_prm_net
            ui->widget_52->setVisible(false); 	//ui->dmo_prd_net
            ui->widget_46->setVisible(false); 	//ui->dmo_n_retr
            ui->widget_57->setVisible(true); 	//ui->dmo_mask
            ui->widget_53->setVisible(ui->dmo_mask->isChecked()); 	//ui->dmo_mask_key
            ui->widget_58->setVisible(true); 	//ui->dmo_name
            return;
        }
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
            ui->tmo_mask_key->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->no_modals){
                ui->channel_editor_left->setText("Сохранить");
                ui->channel_editor_right->setText("Выбрать");
            }
            else{
                ui->channel_editor_left->setText("Выбрать");
                ui->channel_editor_right->setText("Назад");
            }
            break;
        case 8:
            ui->tmo_name->setStyleSheet("border: 2px solid black; background: white;");
            ui->channel_editor_left->setText("Сохранить");
            ui->channel_editor_right->setText("Стереть");
            ui->tmo_name->setFocus();
            break;
        default:
            qCritical("tmo: update_channel_editor_page: no way");
        }

        if(ui->tmo_mask->isChecked()){
            ui->widget_66->setVisible(true);
        }
        else{
            ui->widget_66->setVisible(false);
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

    if(ui->direction_editor_stackedWidget->currentWidget() != ui->direction_tuner_page)
        ui->direction_editor_stackedWidget->setCurrentWidget(ui->direction_tuner_page);
}

void MainWindow::update_direction_editor_page(){
    clear_direction_fields();

    if(chosen_ref_d == nullptr || curr_editor_field["dir"] == 0){
        ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");

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
            if(direction_map[selected_items["direction_list"]].direction->is_idle){
                ui->direction_editor_right->setText("");
            }else{
                ui->direction_editor_right->setText("Выбрать");
            }

        }
        if(chosen_ref_d == nullptr) return;
    }

    Channel* channel = channel_map_d[chosen_ref_d];

    // change buttons for chosen type
    // none
    if(channel->state == 0){
        // was upper
        ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
        return;
    }

    clear_direction_fields();

    // dmo_d
    if(channel->state == 1){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->is_forbidden_prd_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 3:
            ui->priority_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 5:
            ui->timeout_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("dmo_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(false); // tone
        ui->widget_74->setVisible(true); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(true); // timeout
        ui->widget_75->setVisible(false); // eco

        return;
    }

    // tmo_d
    if(channel->state == 2){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->is_forbidden_prd_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 3:
            ui->priority_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("tmo_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(false); // tone
        ui->widget_74->setVisible(true); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(false); // eco

        return;
    }

    // vpd_d
    if(channel->state == 3){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
            break;
        case 1:
            ui->is_forbidden_prd_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Изменить");
            break;
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("vpd_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(false); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(false); // eco

        return;
    }

    // am25_d
    if(channel->state == 4){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
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
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 6:
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("am25_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(true); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(true); // eco

        return;
    }

    // chm25_d
    if(channel->state == 5){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
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
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 6:
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("chm25_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(true); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(true); // eco

        return;
    }

    // chm50_d
    if(channel->state == 6){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
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
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 6:
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("chm50_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(true); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(true); // eco

        return;
    }

    // obp_d
    if(channel->state == 7){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
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
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 6:
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("obp_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(true); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(true); // eco

        return;
    }

    // fm_d
    if(channel->state == 8){
        switch (curr_editor_field["dir"]) {
        case 0:
            // was upper
            ui->channel_in_dir_name->setStyleSheet("border: 2px solid black; background: white;");
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
        case 4:
            ui->scan->setStyleSheet("border: 2px solid black; background: white;");
            if(ui->modals->currentWidget() == ui->scan_modal){
                ui->direction_editor_left->setText("Выбрать");
                ui->direction_editor_right->setText("Назад");
            }
            else{
                ui->direction_editor_left->setText("Сохранить");
                ui->direction_editor_right->setText("Выбрать");
            }
            break;
        case 6:
            ui->economizer->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        case 7:
            ui->name_d->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Стереть");
            ui->name_d->setFocus();
            break;
        case 8:
            ui->background_dir_picture->setStyleSheet("border: 2px solid black; background: white;");
            ui->direction_editor_left->setText("Сохранить");
            ui->direction_editor_right->setText("Выбрать");
            break;
        default:
            qCritical("fm_d: update_direction_editor_page: no way");
        }

        ui->widget_68->setVisible(true); // tone
        ui->widget_74->setVisible(false); // priority
        ui->widget_69->setVisible(true); // scan
        ui->widget_73->setVisible(false); // timeout
        ui->widget_75->setVisible(true); // eco

        return;
    }
}

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
            current_direction->noise++;
            if(current_direction->noise > MAX_NOISE) current_direction->noise = MAX_NOISE;
            ui->label_92->setStyleSheet("image: url(:/resources/volume_" + QString::number(current_direction->noise) + ".png)");
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
        if(ui->modals->currentWidget() == ui->channel_popup_menu){
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
        if(ui->modals->currentWidget() == ui->direction_popup_menu){
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

        // tetra dmo
        if(ui->channel_editor_state->property("chosen") == 1){

            if(ui->modals->currentWidget() == ui->mask_key_popup){
                go_up(ui->mask_key_popup_list, ui->mask_key_popup_list->count());
                update_channel_editor_page();
                return;
            }

            if(ui->modals->currentWidget() == ui->fp_popup){
                go_up(ui->fp_popup_list, ui->fp_popup_list->count());
                update_channel_editor_page();
                return;
            }

            uint sz = editor_fields["dmo"].size();
            curr_editor_field["dmo"] = (curr_editor_field["dmo"] - 1 + sz) % sz;
            while(!dmo_fields.at(curr_editor_field["dmo"])->isVisible()){
                curr_editor_field["dmo"] = (curr_editor_field["dmo"] - 1 + sz) % sz;
            }

            update_channel_editor_page();
            return;
        }

        // tetra tmo
        if(ui->channel_editor_state->property("chosen") == 2){
            if(ui->modals->currentWidget() == ui->mask_key_popup){
                go_up(ui->mask_key_popup_list, ui->mask_key_popup_list->count());
                update_channel_editor_page();
                return;
            }

            uint sz = editor_fields["tmo"].size();
            curr_editor_field["tmo"] = (curr_editor_field["tmo"] - 1 + sz) % sz;

            if(!ui->tmo_mask->isChecked()){
                if(curr_editor_field["tmo"] == 7) curr_editor_field["tmo"]--;
            }

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

        if(ui->modals->currentWidget() == ui->scan_modal){
            go_up(ui->scan_popup, 33);
            return;
        }

        uint sz = editor_fields["dir"].size();
        curr_editor_field["dir"] = (curr_editor_field["dir"] - 1 + sz) % sz;

        while(!direction_fields[curr_editor_field["dir"]]->isVisible()){
            curr_editor_field["dir"] = (curr_editor_field["dir"] - 1 + sz) % sz;
        }

        update_direction_editor_page();
        return;
    }
    if(curr == ui->navigation_page){
        if(ui->modals->currentWidget() == ui->navigation_menu){
            go_up(ui->nav_menu_list, 7);
        }
    }
    if(curr == ui->freq_plans_page){
        if(ui->modals->currentWidget() == ui->fps_menu){
            go_up(ui->fps_popup_menu_list, 3);
            return;
        }
        if(curr_editor_field["freq_plans"]-- == 0) curr_editor_field["freq_plans"]+= 32;
        update_freq_plans_screen();
        return;
    }
    if(curr == ui->freq_plan_page){
        if(ui->modals->currentWidget() == ui->fp_menu){
            go_up(ui->fp_popup_menu_list, 5);
            return;
        }
        go_up(ui->listWidget, ui->listWidget->count());
        //update_freq_plans_screen();
        return;
    }
    if(curr == ui->freq_editor_page){
        if(curr_editor_field["freq_editor"]-- == 0) curr_editor_field["freq_editor"] += 3;
        update_freq_editor_screen();
        return;
    }
    if(curr == ui->keys_list_page){
        if(ui->modals->currentWidget() == ui->keys_list_menu){
            go_up(ui->keys_list_menu_list, 2);
            return;
        }
        if(curr_editor_field["keys_list"]-- == 0) curr_editor_field["keys_list"]+= 32;
        update_keys_list_screen();
        return;
    }
    if(curr == ui->key_editor_page){
        if(ui->modals->currentWidget() == ui->key_editor_menu){
            go_up(ui->key_editor_menu_list, 2);
            return;
        }
        if(curr_editor_field["key_editor"]-- == 0) curr_editor_field["key_editor"] += 8;
        update_key_editor_screen();
    }
}

void go_down(QListWidget* qlw, uint size){
    if(size == 0) return;
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
            if(current_direction != nullptr && current_direction->ch != nullptr && !current_direction->is_idle){
                ui->label_94->setText(current_direction->ch->name);
                ui->atuners->setCurrentWidget(ui->noise);
                noise_show();
            }
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
            current_direction->noise--;
            if(current_direction->noise < MIN_NOISE) current_direction->noise = MIN_NOISE;
            ui->label_92->setStyleSheet("image: url(:/resources/volume_" + QString::number(current_direction->noise) + ".png)");
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
        if(ui->modals->currentWidget() == ui->channel_popup_menu){
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
        if(ui->modals->currentWidget() == ui->direction_popup_menu){
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

        // tetra dmo
        if(ui->channel_editor_state->property("chosen") == 1){

            if(ui->modals->currentWidget() == ui->mask_key_popup){
                go_down(ui->mask_key_popup_list, ui->mask_key_popup_list->count());
                update_channel_editor_page();
                return;
            }

            if(ui->modals->currentWidget() == ui->fp_popup){
                go_down(ui->fp_popup_list, ui->fp_popup_list->count());
                update_channel_editor_page();
                return;
            }

            uint sz = editor_fields["dmo"].size();
            curr_editor_field["dmo"] = (curr_editor_field["dmo"] + 1) % sz;
            while(!dmo_fields.at(curr_editor_field["dmo"])->isVisible()){
                curr_editor_field["dmo"] = (curr_editor_field["dmo"] + 1) % sz;
            }

            update_channel_editor_page();
            return;
        }

        // tetra tmo
        if(ui->channel_editor_state->property("chosen") == 2){

            if(ui->modals->currentWidget() == ui->mask_key_popup){
                go_down(ui->mask_key_popup_list, ui->mask_key_popup_list->count());
                update_channel_editor_page();
                return;
            }

            uint sz = editor_fields["tmo"].size();
            curr_editor_field["tmo"] = (curr_editor_field["tmo"] + 1) % sz;

            if(!ui->tmo_mask->isChecked()){
                if(curr_editor_field["tmo"] == 7) curr_editor_field["tmo"]++;
            }

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

        if(ui->modals->currentWidget() == ui->scan_modal){
            go_down(ui->scan_popup, 33);
            return;
        }

        uint sz = editor_fields["dir"].size();
        curr_editor_field["dir"] = (curr_editor_field["dir"] + 1) % sz;

        while(!direction_fields[curr_editor_field["dir"]]->isVisible()){
            curr_editor_field["dir"] = (curr_editor_field["dir"] + 1) % sz;
        }

        update_direction_editor_page();
        return;
    }
    if(curr == ui->navigation_page){
        if(ui->modals->currentWidget() == ui->navigation_menu){
            go_down(ui->nav_menu_list, 7);
        }
        return;
    }
    if(curr == ui->freq_plans_page){
        if(ui->modals->currentWidget() == ui->fps_menu){
            go_down(ui->fps_popup_menu_list, 3);
            return;
        }
        if(++curr_editor_field["freq_plans"] == 32) curr_editor_field["freq_plans"] = 0;
        update_freq_plans_screen();
        return;
    }
    if(curr == ui->freq_plan_page){
        if(ui->modals->currentWidget() == ui->fp_menu){
            go_down(ui->fp_popup_menu_list, 5);
            return;
        }
        go_down(ui->listWidget, ui->listWidget->count());
        //update_freq_plans_screen();
        return;
    }
    if(curr == ui->freq_editor_page){
        if(++curr_editor_field["freq_editor"] == 3) curr_editor_field["freq_editor"] = 0;
        update_freq_editor_screen();
        return;
    }
    if(curr == ui->keys_list_page){
        if(ui->modals->currentWidget() == ui->keys_list_menu){
            go_down(ui->keys_list_menu_list, 2);
            return;
        }
        if(++curr_editor_field["keys_list"] == 32) curr_editor_field["keys_list"] = 0;
        update_keys_list_screen();
        return;
    }
    if(curr == ui->key_editor_page){
        if(ui->modals->currentWidget() == ui->key_editor_menu){
            go_down(ui->key_editor_menu_list, 2);
            return;
        }
        if(++curr_editor_field["key_editor"] == 8) curr_editor_field["key_editor"] = 0;
        update_key_editor_screen();
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

void MainWindow::check_holded_right_tube(int i){
    if(i == ui->right_tube->property("clicked_times").toInt()){

        auto currPage = ui->mainPages->currentWidget();
        auto currModal = ui->modals->currentWidget();

        // включение после секунды
        if(currPage == ui->offscreen){
            ui->right_tube->setProperty("was_action", true);
            selfcontrol_screen();
            return;
        }

        // пропускание
        if(currPage == ui->selfcontrol_page){
            return;
        }
        if(currPage == ui->loading_page){
            return;
        }

        // предупреждение выключения после секунды
        if(currModal == ui->no_modals){
            ui->right_tube->setProperty("was_action", true);
            ui->modals->setCurrentWidget(ui->shutdown);
            QTimer* t;
            timers.push(t = new QTimer());
            connect(t, &QTimer::timeout, this, [i, t, this]{ check_holded_right_tube(i); t->stop(); });
            t->start(1000);
            return;
        }

        // выключение после ещё 1 секунды
        if(currModal == ui->shutdown){
            ui->right_tube->setProperty("was_action", true);
            ui->modals->setCurrentWidget(ui->no_modals);
            ui->mainPages->setCurrentWidget(ui->offscreen);
            ui->label_107->setVisible(true);
            QTimer *t;
            timers.push(t = new QTimer());
            auto label = ui->label_107;
            connect(t, &QTimer::timeout, this, [t, label](){ label->setVisible(false); t->stop(); });
            t->start(1000);
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
            if(ui->modals->currentWidget() == ui->password){
                ui->modals->setCurrentWidget(ui->no_modals);
                ui->service_menu_left->setText("Выбрать");
                ui->service_menu_right->setText("Назад");
                return;
            }

            QTimer *t;
            timers.push(t = new QTimer());
            ui->modals->setCurrentWidget(ui->wait);
            connect(t, &QTimer::timeout, this, [t, this](){ ui->modals->setCurrentWidget(ui->no_modals); menu_screen(); t->stop(); });
            t->start(1000);
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
            if(ui->modals->currentWidget() == ui->no_modals){
                delete_direction(selected_items["direction_list"]);
                direction_list_screen();
            }
            ui->modals->setCurrentWidget(ui->no_modals);
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
        if(curr == ui->navigation_page){
            if(ui->modals->currentWidget() == ui->navigation_menu){
                ui->modals->setCurrentWidget(ui->no_modals);
                ui->navigation_left->setText("Меню");
                return;
            }
            menu_screen();
            ui->modals->setCurrentWidget(ui->no_modals);
            return;
        }
        if(curr == ui->received_messages_page){
            menu_screen();
            return;
        }
        if(curr == ui->freq_plans_page){
            data_editor_screen();
            return;
        }
        if(curr == ui->freq_plan_page){
            ui->modals->setCurrentWidget(ui->no_modals);
            freq_plans_screen();
            return;
        }
        if(curr == ui->freq_editor_page){
            ui->modals->setCurrentWidget(ui->no_modals);
            freq_plan_screen();
            return;
        }
        if(curr == ui->keys_list_page){
            ui->modals->setCurrentWidget(ui->no_modals);
            data_editor_screen();
        }
        if(curr == ui->key_editor_page){
            ui->modals->setCurrentWidget(ui->no_modals);
            generate = false;
            keys_list_screen();
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


void MainWindow::on_navigation_right_clicked()
{
    if(ui->modals->currentWidget() == ui->navigation_menu){
        ui->modals->setCurrentWidget(ui->no_modals);
        ui->navigation_left->setText("Меню");
        return;
    }
    menu_screen();
}

void MainWindow::on_navigation_left_clicked()
{
    if(ui->modals->currentWidget() != ui->navigation_menu){
        ui->modals->setCurrentWidget(ui->navigation_menu);
        ui->navigation_left->setText("Выбрать");
        return;
    }
}

void MainWindow::on_rec_msgs_right_clicked()
{
    menu_screen();
}

void MainWindow::on_freq_plans_right_clicked()
{
    if(ui->modals->currentWidget() == ui->fps_menu){
        ui->modals->setCurrentWidget(ui->no_modals);
        update_freq_plans_screen();
        return;
    }
    data_editor_screen();
}

void MainWindow::on_freq_plans_left_clicked()
{
    if(ui->modals->currentWidget() == ui->fps_menu){
        // CHECK
        if(selected_items["fps_popup_menu_list"] == fps_popup_menu_list_item[0]){
            if(freq_plan_vec[curr_editor_field["freq_plans"]] == nullptr){
                return;
            }
            ui->modals->setCurrentWidget(ui->no_modals);
            freq_plan_screen();
        }
        // ADD
        if(selected_items["fps_popup_menu_list"] == fps_popup_menu_list_item[1]){
            ui->modals->setCurrentWidget(ui->no_modals);
            if(freq_plan_vec[curr_editor_field["freq_plans"]] == nullptr){
                freq_plan_vec[curr_editor_field["freq_plans"]] = new FreqPlan();
                dynamic_cast<QLabel*>(freq_plans[curr_editor_field["freq_plans"]]->children()[1])->setText("");
                dynamic_cast<QLabel*>(freq_plans[curr_editor_field["freq_plans"]]->children()[2])->setText(QString("%1 Частотный план").arg(curr_editor_field["freq_plans"] + 1));
            }
            freq_plan_screen();
        }
        // DELETE
        if(selected_items["fps_popup_menu_list"] == fps_popup_menu_list_item[2]){

        }
        return;
    }

    ui->modals->setCurrentWidget(ui->fps_menu);
    update_freq_plans_screen();
}

void MainWindow::update_freq_plans_screen(){
    if(ui->modals->currentWidget() == ui->fps_menu){
        ui->freq_plans_left->setText("Выбрать");
        return;
    }

    ui->freq_plans_left->setText("Меню");

    // clear styles
    for(int i = 0; i < 32; i++){
        freq_plans[i]->setStyleSheet("");
    }
    // paint chosen
    freq_plans[curr_editor_field["freq_plans"]]->setStyleSheet("background: rgb(62, 105, 194);");

    ui->scrollArea_2->ensureWidgetVisible(freq_plans[curr_editor_field["freq_plans"]]);
}

void MainWindow::update_keys_list_screen(){
    if(ui->modals->currentWidget() == ui->keys_list_menu){
        ui->keys_list_left->setText("Выбрать");
        return;
    }

    ui->keys_list_left->setText("Меню");

    // clear styles
    for(int i = 0; i < 32; i++){
        keys_list[i]->setStyleSheet("");
    }
    // paint chosen
    keys_list[curr_editor_field["keys_list"]]->setStyleSheet("background: rgb(62, 105, 194);");

    ui->scrollArea_3->ensureWidgetVisible(keys_list[curr_editor_field["keys_list"]]);
}

void MainWindow::on_freq_plan_right_clicked()
{
    if(ui->modals->currentWidget() == ui->fp_menu){
        ui->modals->setCurrentWidget(ui->no_modals);
        //update_freq_plan_screen();
        ui->freq_plan_left->setText("Меню");
        return;
    }
    freq_plans_screen();
}

void MainWindow::on_freq_plan_left_clicked()
{
    if(ui->modals->currentWidget() == ui->fp_menu){
        // EDIT
        if(selected_items["fp_popup_menu_list"] == fp_popup_menu_list_item[0]){

        }
        // ADD RANGE
        if(selected_items["fp_popup_menu_list"] == fp_popup_menu_list_item[1]){
            ui->modals->setCurrentWidget(ui->no_modals);            
            freq_editor_screen();
        }
        // DELETE RANGE
        if(selected_items["fp_popup_menu_list"] == fp_popup_menu_list_item[2]){

        }
        // TEST a PLAN
        if(selected_items["fp_popup_menu_list"] == fp_popup_menu_list_item[3]){

        }
        // AUTOFORMATE
        if(selected_items["fp_popup_menu_list"] == fp_popup_menu_list_item[4]){

        }
        return;
    }

    ui->modals->setCurrentWidget(ui->fp_menu);
    ui->freq_plan_left->setText("Выбрать");
}

void MainWindow::on_freq_editor_right_clicked()
{
    switch (curr_editor_field["freq_editor"]) {
    case 0:
        ui->lineEdit_2->backspace();
        break;
    case 1:
        ui->lineEdit_3->backspace();
        break;
    case 2:
        ui->pushButton->click();
        break;
    default:
        qDebug() << "crit: on_freq_editor_right_clicked";
    }
    update_freq_editor_screen();
}

void MainWindow::on_freq_editor_left_clicked()
{
    save_freq_range();
    if(ui->modals->currentWidget() == ui->params_error){
        return;
    }
    update_fp_popup_list();
    freq_plan_screen();
}

void MainWindow::on_pushButton_clicked()
{
    // check ranges
    int left = ui->lineEdit_2->text().toInt();
    int right = ui->lineEdit_3->text().toInt();

    if(!in_range(left, 27000000, 520000000) || !in_range(right, 27000000, 520000000) || left > right){
        ui->label_168->setText("Ошибка. Допустимый\nдиапазон 27..520 МГц");
        return;
    }

    // print result
    // kinda rssi calculation
    double mid = (left + right) / 2000000.;
    std::stringstream ss;
    double rssi = -1./4500 * pow(mid - 520, 2) - 50;
    int dist = (int)(-1./4100 * pow(mid - 520, 2) + 100);
    ss << std::fixed << std::setprecision(3) << "Средний " << rssi << " дБм\nДальность " << dist << "%";
    ui->label_168->setText(ss.str().c_str());
}

void MainWindow::update_freq_editor_screen(){
    // clear styles
    ui->widget_81->setStyleSheet("#widget_81 { background: white; }");
    ui->widget_82->setStyleSheet("#widget_82 { background: white; }");
    ui->pushButton->setStyleSheet("border: 1px solid;");
    ui->label_166->setVisible(false);
    ui->label_167->setVisible(false);

    int left, right;
    left = (ui->lineEdit_2->text().toInt() / 10000) * 10000;
    right = (ui->lineEdit_3->text().toInt() / 10000) * 10000;

    switch (curr_editor_field["freq_editor"]) {
    case 0:
        ui->widget_81->setStyleSheet("#widget_81 { background: white; border: 2px solid; }");
        ui->label_166->setVisible(true);
        ui->freq_editor_right->setText("Стереть");

        ui->lineEdit_3->setText(QString::number(right));
        break;
    case 1:
        ui->widget_82->setStyleSheet("#widget_82 { background: white; border: 2px solid; }");
        ui->label_167->setVisible(true);
        ui->freq_editor_right->setText("Стереть");

        ui->lineEdit_2->setText(QString::number(left));
        break;
    case 2:
        ui->pushButton->setStyleSheet("border: 2px solid;");
        ui->freq_editor_right->setText("Тестировать");

        ui->lineEdit_2->setText(QString::number(left));
        ui->lineEdit_3->setText(QString::number(right));
        break;
    default:
        qDebug() << "crit: on_freq_editor_right_clicked";
    }
}

void MainWindow::save_freq_range(){
    int left = ui->lineEdit_2->text().toInt();
    int right = ui->lineEdit_3->text().toInt();

    if(!in_range(left, 27000000, 520000000) || !in_range(right, 27000000, 520000000) || left > right){
        ui->modals->setCurrentWidget(ui->params_error);
        return;
    }

    // saving
    auto t = new FreqRange();
    t->lower_freq = left;
    t->upper_freq = right;
    double mid = (left + right) / 2000000.;
    t->rssi = -1./4500 * pow(mid - 520, 2) - 50;
    t->dist = (int)(-1./4100 * pow(mid - 520, 2) + 100);
    freq_plan_vec[curr_editor_field["freq_plans"]]->ranges.push_back(t);    
}

void MainWindow::on_keys_list_right_clicked()
{
    if(ui->modals->currentWidget() != ui->no_modals){
        ui->modals->setCurrentWidget(ui->no_modals);
        return;
    }
    data_editor_screen();
}

void MainWindow::on_keys_list_left_clicked()
{
    if(ui->modals->currentWidget() == ui->keys_list_menu){
        // EDIT / ADD
        if(selected_items["keys_list_menu_list"] == keys_list_menu_list_item[0]){
            ui->modals->setCurrentWidget(ui->no_modals);
            key_editor_screen();
        }

        // DELETE
        if(selected_items["keys_list_menu_list"] == keys_list_menu_list_item[1]){

        }
        return;
    }
    ui->modals->setCurrentWidget(ui->keys_list_menu);
}

void MainWindow::key_editor_screen(){
    ui->mainPages->setCurrentWidget(ui->key_editor_page);

    ui->lineEdit_4->setText("0");
    ui->lineEdit_5->setText("0");
    ui->lineEdit_6->setText("0");
    ui->lineEdit_7->setText("0");
    ui->lineEdit_8->setText("0");
    ui->lineEdit_9->setText("0");
    ui->lineEdit_10->setText("0");
    ui->lineEdit_11->setText("0");

    ui->label_184->setText("0");
    ui->label_187->setText("0");
    ui->label_193->setText("0");
    ui->label_175->setText("0");
    ui->label_190->setText("0");
    ui->label_178->setText("0");
    ui->label_181->setText("0");
    ui->label_196->setText("0");

    ui->label_172->setText("CRC: 0");

    update_key_editor_screen();
}

void MainWindow::update_key_editor_screen(){
    // clear
    ui->widget_97->setStyleSheet("background: white;");
    ui->widget_90->setStyleSheet("background: white;");
    ui->widget_91->setStyleSheet("background: white;");
    ui->widget_92->setStyleSheet("background: white;");
    ui->widget_93->setStyleSheet("background: white;");
    ui->widget_94->setStyleSheet("background: white;");
    ui->widget_95->setStyleSheet("background: white;");
    ui->widget_96->setStyleSheet("background: white;");

    if(ui->modals->currentWidget() == ui->key_editor_menu){
        ui->key_editor_left->setText("Выбрать");
        ui->key_editor_right->setText("Назад");
        if(generate == true){
            key_editor_menu_list_item[1]->setIcon(QIcon(":/resources/green-ok.png"));
        }
        else{
            key_editor_menu_list_item[1]->setIcon(QIcon(""));
        }
    }
    else{
        ui->key_editor_left->setText("Меню");
        if(generate == true) {
            ui->key_editor_right->setText("Стоп");
        }
        else{
            ui->key_editor_right->setText("Стереть");
        }
    }

    auto k = new Key();
    for(int i = 0; i < 8; i++){
        k->values[i] = dynamic_cast<QLineEdit*>(key_editor[i]->children()[2])->text();
        k->values2[i] = dynamic_cast<QLabel*>(key_editor[i]->children()[3])->text().toInt();
        k->crc += k->values[i].toInt();
    }
    k->crc %= 521;

    ui->label_172->setText(QString("CRC: %1").arg(k->crc));
    delete k;
    k = nullptr;

    key_editor[curr_editor_field["key_editor"]]->setStyleSheet("background: #387ef3;");
}

void MainWindow::on_key_editor_right_clicked()
{
    if(ui->modals->currentWidget() == ui->key_editor_menu){
        ui->modals->setCurrentWidget(ui->no_modals);
        update_key_editor_screen();
        return;
    }
    if(generate){

        return;
    }

    switch (curr_editor_field["key_editor"]) {
    case 0:
        ui->lineEdit_4->backspace();
        break;
    case 1:
        ui->lineEdit_5->backspace();
        break;
    case 2:
        ui->lineEdit_6->backspace();
        break;
    case 3:
        ui->lineEdit_7->backspace();
        break;
    case 4:
        ui->lineEdit_8->backspace();
        break;
    case 5:
        ui->lineEdit_9->backspace();
        break;
    case 6:
        ui->lineEdit_10->backspace();
        break;
    case 7:
        ui->lineEdit_11->backspace();
        break;
    }
}

void MainWindow::update_mask_key_popup_list(){
    ui->mask_key_popup_list->clear();
    ui->mask_key_popup_list->addItem("Не задано");
    for(int i = 0; i < 32; i++){
        if(keys_vec[i]){
            ui->mask_key_popup_list->addItem(QString("%1 Ключ").arg(i+1));
        }
    }
    ui->mask_key_popup_list->setCurrentRow(0);
}

void MainWindow::on_key_editor_left_clicked()
{
    if(ui->modals->currentWidget() == ui->key_editor_menu){
        // SAVE
        if(ui->key_editor_menu_list->currentItem() == key_editor_menu_list_item[0]){
            auto k = new Key();
            for(int i = 0; i < 8; i++){
                k->values[i] = dynamic_cast<QLineEdit*>(key_editor[i]->children()[2])->text();
                k->values2[i] = dynamic_cast<QLabel*>(key_editor[i]->children()[3])->text().toInt();
                k->crc += k->values[i].toInt();
            }
            k->crc %= 521;

            keys_vec[curr_editor_field["keys_list"]] = k;

            update_mask_key_popup_list();

            keys_list_screen();
        }
        // GEN
        if(ui->key_editor_menu_list->currentItem() == key_editor_menu_list_item[1]){
            generate = !generate;
        }
        ui->modals->setCurrentWidget(ui->no_modals);
        update_key_editor_screen();
        return;
    }
    ui->modals->setCurrentWidget(ui->key_editor_menu);
    update_key_editor_screen();
    return;
}

void MainWindow::update_fp_popup_list(){
    ui->fp_popup_list->clear();
    ui->fp_popup_list->addItem("Не задано");
    for(int i = 0; i < 32; i++){
        if(freq_plan_vec[i]){
            ui->fp_popup_list->addItem(QString("%1 ЧП").arg(i+1));
        }
    }
    ui->fp_popup_list->setCurrentRow(0);
}
