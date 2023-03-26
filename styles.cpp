#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QScreen>
#include "listviewdelegate.h"

void MainWindow::set_styles()
{
    double scale = 1.0;

#if (defined (_WIN32) || defined (_WIN64))
    scale = QGuiApplication::primaryScreen()->logicalDotsPerInchY() / 96;
#endif

    QFontDatabase::addApplicationFont(":/resources/Nokia Sans Wide.ttf");
    QFontDatabase::addApplicationFont(":/resources/Nokia Sans Wide Bold.ttf");
    QFont f("Nokia Sans Wide", 12 / scale, QFont::Normal);
    QFont fb("Nokia Sans Wide", 12 / scale, QFont::Bold);
    QFont scsf("Nokia Sans Wide", 10 / scale, QFont::Bold);
    QFont lsf("Nokia Sans Wide", 10 / scale, QFont::Normal);
    QFont fpop("Nokia Sans Wide", 9 / scale, QFont::Normal);
    // QFont minf("Nokia Sans Wide", 20 / scale, QFont::Bold);
    // QFont secf("Nokia Sans Wide", 10 / scale, QFont::Bold);

    // Service menu page
    ui->label_4->setFont(fb);
    ui->service_menu_list->setFont(f);
    ui->service_menu_left->setFont(fb);
    ui->service_menu_right->setFont(fb);

    // Channel list page
    ui->label_8->setFont(fb);
    ui->channel_list->setFont(f);
    ui->channel_popup_menu_list->setFont(fpop);
    ui->empty_channel_list_label->setFont(f);
    ui->channel_list_left->setFont(fb);
    ui->channel_list_right->setFont(fb);
    ui->label_8->setFont(fb);

    // Channel editor page
    ui->label_6->setFont(fb);
    ui->channel_editor_left->setFont(fb);
    ui->channel_editor_right->setFont(fb);

    // Direction editor page
    ui->label_11->setFont(f);
    ui->direction_editor_left->setFont(fb);
    ui->direction_editor_right->setFont(fb);
    ui->channel_in_dir_name->setFont(f);
    ui->is_forbidden_prd_d->setFont(f);
    ui->is_tone_call->setFont(f);
    ui->label_24->setFont(f);
    ui->label_25->setFont(f);
    ui->label_26->setFont(f);
    ui->label_33->setFont(f);
    ui->label_34->setFont(f);
    ui->scan->setFont(f);
    ui->label_35->setFont(f);
    ui->label_36->setFont(f);
    ui->name_d->setFont(f);
    ui->channel_choice_list->setFont(f);
    ui->label_21->setFont(fb);

    // Data editor page
    ui->label_5->setFont(fb);
    ui->data_editor_list->setFont(f);
    ui->data_editor_left->setFont(fb);
    ui->data_editor_right->setFont(fb);
    ui->economizer->setFont(f);

    // Direction list page
    ui->label_9->setFont(fb);
    ui->direction_list->setFont(f);
    ui->direction_popup_menu_list->setFont(fpop);
    ui->direction_list_left->setFont(fb);
    ui->direction_list_right->setFont(fb);

    // Menu page
    ui->label_dej_2->setFont(fb);
    ui->menu_list->setFont(f);
    ui->menu_left->setFont(fb);
    ui->menu_right->setFont(fb);

    // Direction selection page
    ui->label_40->setFont(fb);
    ui->direction_selection_left->setFont(fb);
    ui->direction_selection_right->setFont(fb);

    // Main page
    ui->label_dej_1->setFont(fb);
    // ui->hours_minutes_2->setFont(minf);
    // ui->seconds_2->setFont(secf);
    ui->main_left->setFont(fb);
    ui->main_right->setFont(fb);
    ui->channel_label->setFont(fb);
    ui->data_label->setFont(fb);
    ui->direction_label->setFont(fb);
    ui->open_state_label->setFont(fb);

    // Self-Control page
    ui->label_12->setFont(scsf);
    ui->label_CPU->setFont(fb);
    ui->label_CPU_r->setFont(scsf);
    ui->label_CRAM->setFont(fb);
    ui->label_CRAM_r->setFont(scsf);
    ui->label_DSP->setFont(fb);
    ui->label_DSP_r->setFont(scsf);
    ui->label_Dostup->setFont(fb);
    ui->label_Dostup_r->setFont(scsf);
    ui->label_FLASH->setFont(fb);
    ui->label_FLASH_r->setFont(scsf);
    ui->label_Klapan->setFont(fb);
    ui->label_Klapan_r->setFont(scsf);
    ui->label_MPU->setFont(fb);
    ui->label_MPU_r->setFont(scsf);
    ui->label_PRAM->setFont(fb);
    ui->label_PRAM_r->setFont(scsf);
    ui->label_Program->setFont(fb);
    ui->label_Program_r->setFont(scsf);
    ui->label_SDRAM->setFont(fb);
    ui->label_SDRAM_r->setFont(scsf);
    ui->label_Zagruz4ik->setFont(fb);
    ui->label_Zagruz4ik_r->setFont(scsf);

    // Loading page
    ui->label_13->setFont(scsf);
    ui->label_14->setFont(lsf);
    ui->label_15->setFont(lsf);
    ui->label_17->setFont(lsf);
    ui->label_18->setFont(lsf);
    ui->label_19->setFont(lsf);
    ui->label_20->setFont(lsf);

    // List styles
    ui->service_menu_list->setItemDelegate(new ListWidgetItemDelegate);
    ui->menu_list->setItemDelegate(new ListWidgetItemDelegate(0.2, 0));
    ui->data_editor_list->setItemDelegate(new ListWidgetItemDelegate);
}
