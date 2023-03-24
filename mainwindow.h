#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QListWidget"
#include <map>
#include <tuple>
#include <QUdpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <qbuffer.h>
#include "modalwindowrd.h"
#include "modalwindow3d.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ModalWindowRD rdwin;
    ModalWindow3D _3dwin;

public slots:
    void change_global_time();

public Q_SLOTS:

    void setup();
    void off_screen();
    void selfcontrol_screen();
    void loading_screen();
    void main_screen();
    void menu_screen();
    void service_menu_screen();
    void data_editor_screen();

    void update_channel_list_screen();
    void update_direction_list_screen();

    void channel_list_screen();
    void direction_list_screen();

    void channel_editor_screen();
    void direction_editor_screen();

    void direction_selection_screen();

    void broadcast_init();

private:
    QAction *show3d;
    QAction *showrd;
    QAction *shownorm;

    void sendDatagrams();
    void playSamples();

    inline int getFreq();

    template<typename T>
    inline void to_byte_array(char a[], T t)
    {
        memcpy(a, &t, sizeof(T));
    }

    template<typename T>
    inline void from_byte_array(const char a[], T& t)
    {
        memcpy(&t, a, sizeof(T));
    }

    void setReceiving();
    void setTransmitting();

    size_t receivedPackets = 0;

private slots:

    void hideDej();

    void show_3d();

    void show_radiodata();

    void show_normative();

    void set_styles();

    void recieveDatagrams();

    void on_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_menu_list_itemSelectionChanged();

    void on_service_menu_list_itemSelectionChanged();

    void on_service_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_data_editor_list_itemDoubleClicked(QListWidgetItem *item);

    void on_service_menu_left_clicked(); //
    void on_service_menu_right_clicked(); //

    void on_channel_list_left_clicked(); //
    void on_channel_list_right_clicked(); //

    void on_direction_list_left_clicked(); //
    void on_direction_list_right_clicked(); //

    void on_data_editor_left_clicked(); //
    void on_data_editor_right_clicked(); //

    void on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_channel_editor_left_clicked(); //
    void on_channel_editor_right_clicked(); //

    void on_channel_list_itemClicked(QListWidgetItem *item);

    void on_direction_popup_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_channel_choice_list_itemDoubleClicked(QListWidgetItem *item);

    void on_direction_editor_left_clicked(); //
    void on_direction_editor_right_clicked(); //

    void on_channel_in_dir_name_clicked();

    void on_channel_choice_list_itemClicked(QListWidgetItem *item);

    void on_direction_list_itemClicked(QListWidgetItem *item);

    void on_main_left_clicked(); //
    void on_main_right_clicked(); //

    void on_menu_left_clicked(); //
    void on_menu_right_clicked(); //

    void on_direction_selection_left_clicked(); //
    void on_direction_selection_right_clicked(); //

    void on_direction_selection_list_itemClicked(QListWidgetItem *item);

    //void on_direction_button_clicked();

    void on_left_arrow_clicked();
    void on_right_arrow_clicked();

    void on_number_1_clicked();
    void on_number_2_clicked();
    void on_number_3_clicked();
    void on_number_4_clicked();
    void on_number_5_clicked();
    void on_number_6_clicked();
    void on_number_7_clicked();
    void on_number_8_clicked();
    void on_number_9_clicked();
    void on_number_0_clicked();

    void on_up_arrow_clicked();

    void on_down_arrow_clicked();

    void on_left_tube_clicked();

    void on_right_tube_clicked();

    void on_data_editor_list_itemSelectionChanged();

    void on_channel_popup_menu_list_itemSelectionChanged();

    void on_direction_popup_menu_list_itemSelectionChanged();

    void on_direction_selection_list_itemSelectionChanged();

    void on_chm25_dualfreq_clicked();

    void on_channel_list_itemSelectionChanged();

    void _on_channel_editor_state_popup_itemSelectionChanged();

    void _on_channel_editor_ctcss_popup_itemSelectionChanged();

    void _on_direction_editor_scan_popup_itemSelectionChanged();

    void on_direction_list_itemSelectionChanged();

    void on_channel_choice_list_itemSelectionChanged();

    void _on_economizer_numChanged();

    void _on_direction_background_numChanged();

    void on_talk_button_pressed();

private:
    void on_number_i_clicked(int);
    void clear_chm25_fields();
    void clear_chm25_d_fields();
    void clear_chm50_fields();
    void clear_am25_fields();
    void clear_tetra_tmo_fields();
    void clear_vpd_fields();
    void clear_obp_fields();
    void clear_fm_fields();
    void update_channel_editor_page();
    void update_direction_editor_page();
    void swap_direction_page();
    QPoint global_pos(QWidget*);
    void set_default_channel_fields();
    void set_default_direction_fields();

private:
    struct Channel;
    struct Direction;

    Ui::MainWindow *ui;

    using ref = QListWidgetItem*;

    // a current list item of all QListWidgets
    std::map<QString, ref> selected_items;

    // a number of a current field in a specified editor screen
    std::map<QString, uint> curr_editor_field;
    // fields
    std::map<QString, std::vector<QString>> editor_fields;
    // types
    std::vector<QString> channel_types;

    std::vector<QString> direction_types;

    bool is_open_communication = true;

    static const uint menu_list_size = 7;
    ref menu_list_item[menu_list_size];
    static const uint service_menu_list_size = 9;
    ref service_menu_list_item[service_menu_list_size];
    static const uint data_editor_list_size = 9;
    ref data_editor_list_item[data_editor_list_size];

    struct channelData
    {
        Channel* channel;
        ref ref2; // reference to a row in direction_selection_list
    };

    struct directionData
    {
        Direction* direction;
        ref ref2; // reference to a row in direction_selection_list
    };

    /*
    {channel_map}   ref  -> (Channel* , ref2)
    {channel_map_d} ref2 -> Channel*
    */
    std::map<ref, channelData> channel_map;
    std::map<ref, Channel*> channel_map_d;
    std::map<ref, directionData> direction_map;
    std::map<ref, Direction*> direction_map_d;

    QListWidgetItem* channel_popup_menu_list_item[3];
    QListWidgetItem* direction_popup_menu_list_item[3];

    QTimer time_timer;

    Direction* current_direction = nullptr;

    QListWidget* channel_editor_state_popup;
    QListWidget* channel_editor_ctcss_popup;
    QListWidget* direction_editor_scan_popup;

    QListWidgetItem* channel_editor_state_popup_item[9];
    QListWidgetItem* channel_editor_ctcss_popup_item[65];

    //kostil
    ref chosen_ref_d = 0;


    QUdpSocket udpSocket;
    const QString ADDR = "26.115.163.75";
    const uint PORT = 52130;
    bool transmitting = false;
    QAudioInput* inpt;
    QAudioOutput* outp;
    QIODevice* inptDev;
    QIODevice* outpDev;
    QByteArray buffer;
    QMetaObject::Connection inptConn;
    QMetaObject::Connection outpConn;
    const uint BUF_SZ = 1024;
    char freq_bytes[4];

protected:

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

struct MainWindow::Channel
{
    quint32 state = 0; //"Не задано"
    bool PRD = false;
    bool dualfreq = false;
    quint32 freq = 0;
    quint32 prm_freq = 0;
    quint32 prd_freq = 0;
    quint32 ctcss = 0;
    QString name = "";

    // set default all fields
    void clear(){
        // dmo
        // tmo
        // am25
        // vpd
        // chm25
        // chm50

        state = 0; //"Не задано"
        PRD = false;
        dualfreq = false;
        freq = 0;
        prm_freq = 0;
        prd_freq = 0;
        ctcss = 0;
        name = "";

        // obp
        // fm
    }
};

struct MainWindow::Direction
{
    Channel* ch = nullptr;
    bool PRD = false;
    bool tone_call = false;
    quint32 scan_list = 0; //32
    qint32 economizer = 3; //4
    QString name = "";
    quint32 background = 0; //?

    // set default all fields
    void clear(){
        ch = nullptr;
        PRD = false;
        tone_call = false;
        scan_list = 0;
        economizer = 3;
        name = "";
        background = 0;
    }
};

#endif // MAINWINDOW_H
