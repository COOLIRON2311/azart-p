#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "QTimer"
#include "QListWidget"
#include <map>
#include <tuple>
#include <queue>
#include <QTcpSocket>
#include <QAudioInput>
#include <QAudioOutput>
#include <qbuffer.h>
#include "modalwindowrd.h"
#include "modalwindow3d.h"
#include "modalwindownorm.h"
#include "modalwindowrules.h"
#include "protocol.h"
#include <QList>
#include <set>
#include <iomanip>
#include <QSoundEffect>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Header self; // own config
    Header corr; // correspondent

    QSoundEffect noise_effect;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ModalWindowRD rdwin;
    ModalWindow3D _3dwin;    
    ModalWindowNorm normwin;
    ModalWindowRules ruleswin;

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

    void navigation_screen();
    void received_messages_screen();

    void RS485_PRM_screen();
    void RS485_PRD_screen();
    void USB_PRM_screen();
    void BL_PRM_screen();
    void BL_PRD_screen();
    void IR_PRM_screen();
    void IR_PRD_screen();

    void freq_plans_screen();
    void freq_plan_screen();
    void freq_editor_screen();
    void keys_list_screen();
    void key_editor_screen();

    void update_channel_list_screen();
    void update_direction_list_screen();
    void update_freq_plans_screen();
    void update_freq_editor_screen();
    void update_keys_list_screen();
    void update_key_editor_screen();

    void update_mask_key_popup_list();
    void update_fp_popup_list();

    void channel_list_screen();
    void direction_list_screen();

    void channel_editor_screen();
    void direction_editor_screen();

    void direction_selection_screen();

    void broadcast_init();

    void update_noise();

private:
    char _buf[sizeof (Header)];
    QAction *show3d;
    QAction *showrd;
    QAction *shownorm;
    QAction *showrules;

    ///0: chp_dmo, 1: chp_retr, 2: chp_prd, 3: prm_net, 4: prd_net
    void set_header_fr(int idx, int value);
    void set_header();
    /// 0 - configs match, 1 - partial match, -1 - no match
    inline int _check_mode_params();
    /// 0 - configs match, 1 - partial match, -1 - no match
    inline int compare_configs();
    void sendData();
    void playSamples();

    void setReceiving();
    void setTransmitting();
    void reset_socket();

    size_t receivedPackets = 0;
    size_t receivedNoise = 0;

private slots:
    void receiveData();

    void hide_dej_labels();

    void show_dej_labels();

    void hideDej();

    void show_3d();

    void show_radiodata();

    void show_normative();

    void show_rules();

    void set_styles();

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

    bool delete_channel(QListWidgetItem *item);
    void delete_direction(QListWidgetItem *item);

    void on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_channel_editor_left_clicked(); //
    void on_channel_editor_right_clicked(); //

    void on_channel_list_itemClicked(QListWidgetItem *item);

    void on_direction_popup_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_channel_choice_list_itemDoubleClicked(QListWidgetItem *item);

    void on_direction_editor_left_clicked(); //
    void on_direction_editor_right_clicked(); //

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

    void on_data_editor_list_itemSelectionChanged();

    void on_channel_popup_menu_list_itemSelectionChanged();

    void on_direction_popup_menu_list_itemSelectionChanged();

    void on_direction_selection_list_itemSelectionChanged();

    void on_chm25_dualfreq_clicked();

    void on_channel_list_itemSelectionChanged();

    void _on_channel_editor_state_popup_itemSelectionChanged();

    void on_direction_list_itemSelectionChanged();

    void on_channel_choice_list_itemSelectionChanged();

    void _on_economizer_numChanged();

    void _on_direction_background_numChanged();

    void on_talk_button_pressed();

    void on_right_tube_pressed();

    void on_right_tube_released();

    void check_holded_right_tube(int);

    void volume_show();
    void noise_show();

    void volume_handler(int);
    void noise_handler(int);

    void on_RS485_PRM_right_clicked();

    void on_USB_PRM_right_clicked();

    void on_RS485_PRD_right_clicked();

    void on_BL_PRM_right_clicked();

    void on_talk_button_released();

    void on_navigation_right_clicked();

    void on_navigation_left_clicked();

    void on_nav_menu_list_itemSelectionChanged();

    void on_rec_msgs_right_clicked();

    void on_scan_popup_itemSelectionChanged();

    void on_ctcss_popup_itemSelectionChanged();

    void on_freq_plans_right_clicked();

    void on_freq_plans_left_clicked();

    void on_fps_popup_menu_list_itemSelectionChanged();

    void on_fp_popup_menu_list_itemSelectionChanged();

    void on_freq_plan_right_clicked();

    void on_freq_plan_left_clicked();

    void on_freq_editor_right_clicked();

    void on_freq_editor_left_clicked();

    void on_pushButton_clicked();

    void on_keys_list_right_clicked();

    void on_keys_list_menu_list_itemSelectionChanged();

    void on_keys_list_left_clicked();

    void on_key_editor_menu_list_itemSelectionChanged();

    void on_key_editor_right_clicked();

    void on_key_editor_left_clicked();

    void on_mask_key_popup_list_itemSelectionChanged();

    void on_fp_popup_list_itemSelectionChanged();

    void on_network_popup_list_itemSelectionChanged();

private:
    void readIP();
    void on_number_i_clicked(int);
    void clear_chm25_fields();
    void clear_direction_fields();
    void clear_chm50_fields();
    void clear_am25_fields();
    void clear_tetra_dmo_fields();
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

    QString getFormatFreq(int freq);
    QString getNewDirectionName();

    void save_freq_range();

private:
    struct Channel;
    struct Direction;

    struct FreqPlan;
    struct FreqRange;

    struct Key;

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
    QListWidgetItem* fps_popup_menu_list_item[3];
    QListWidgetItem* fp_popup_menu_list_item[5];
    QListWidgetItem* keys_list_menu_list_item[2];
    QListWidgetItem* key_editor_menu_list_item[2];

    QTimer time_timer;

    Direction* current_direction = nullptr;

    QListWidget* channel_editor_state_popup;
    QListWidget* channel_editor_ctcss_popup;
    QListWidget* direction_editor_scan_popup;

    QListWidgetItem* channel_editor_state_popup_item[9];
    QListWidgetItem* channel_editor_ctcss_popup_item[65];

    //kostil
    ref chosen_ref_d = 0;
    std::queue<QTimer*> timers;
    QList<QWidget*> dmo_fields;
    QList<QWidget*> direction_fields;
    QList<QWidget*> freq_plans;
    QList<QWidget*> freq_editor;
    QList<QWidget*> keys_list;
    QList<QWidget*> key_editor;

    std::vector<FreqPlan*> freq_plan_vec = std::vector<FreqPlan*>(32, nullptr);
    std::vector<Key*> keys_vec = std::vector<Key*>(32, nullptr);


    QTcpSocket *sock;
    QString ADDR;
    const uint PORT = 52130;
    bool transmitting = false;
    QAudioInput* inpt;
    QAudioOutput* outp;
    QIODevice* inptDev;
    QIODevice* outpDev;
    QByteArray buffer;
    QMetaObject::Connection inptConn;
    QMetaObject::Connection outpConn;
    const uint BUF_SZ = 4096;

    int volume = 6;
    int MAX_VOLUME = 16;
    int MAX_NOISE = 16;
    int MIN_VOLUME = 1;
    int MIN_NOISE = 0;

    bool generate = false;
protected:

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

struct MainWindow::Channel
{
    Mode mode = Mode::None;
    std::set<Direction*> used_by;
    bool is_new = true;
    quint32 state = 0; //"Не задано"
    bool PRD = false;
    bool dualfreq = false;
    quint32 freq = 0;
    quint32 band = 0;
    quint32 prm_freq = 0;
    quint32 prd_freq = 0;
    quint32 ctcss = 0;
    QString name = "";
    QString mcc = "";
    QString mnc = "";
    QString gssi = "";

    bool vesh = false;
    bool mask = false;

    bool pprch = false;
    bool retr = false;
    bool prd_only_retr = false;
    QString sos = "";
    QString f_dmo = "";
    QString f_retr = "";
    QString tx_dmo = "";

    int chp_dmo = 0;
    int chp_retr = 0;
    int chp_prd = 0;
    QString prm_hz = "";
    QString prd_hz = "";
    int prm_net = 0;
    int prd_net = 0;
    QString n_retr = "";
    int mask_key = 0;

    int net = 0;

    // set default all fields
    void clear(){
        mode = Mode::None;
        state = 0; //"Не задано"
        PRD = false;
        dualfreq = false;
        freq = 0;
        band = 1; // Верхняя
        prm_freq = 0;
        prd_freq = 0;
        ctcss = 0;
        name = "";
        mcc = "250";
        mnc = "1";
        gssi = "";

        vesh = false;
        mask = false;

        pprch = false;
        retr = false;
        prd_only_retr = false;
        sos = "";
        f_dmo = "";
        f_retr = "";
        tx_dmo = "";

        chp_dmo = 0;
        chp_retr = 0;
        chp_prd = 0;
        prm_hz = "";
        prd_hz = "";
        prm_net = 0;
        prd_net = 0;
        n_retr = "";
        mask_key = 0;

        net = 0;
    }
};

struct MainWindow::Direction
{
    bool is_new = true;
    bool is_idle = false;
    int noise = 2;
    int noiselevel = rand() % 4 + 4;

    Channel* ch = nullptr;
    bool PRD = false;
    bool tone_call = false;
    quint32 scan_list = 0; //0 - 32
    qint32 economizer = 3; //0 - 3
    QString name = "";
    quint32 background = 0; //1 - 10
    int priority = 0;
    int timeout = 0;

    // set default all fields
    void clear(){
        ch = nullptr;
        PRD = false;
        tone_call = false;
        scan_list = 0;
        economizer = 3;
        name = "";
        background = 0;
        priority = 0;
        timeout = 0;
    }
};

struct MainWindow::FreqPlan{
    std::vector<FreqRange*> ranges;
};

static std::string freqTo2block(int f){
    char buf[10] = "";
    sprintf(buf, "%03d.%03d", f/1000000, (f/1000) % 1000);
    return std::string(buf);
}

struct MainWindow::FreqRange{
    int lower_freq;
    int upper_freq;
    double rssi;
    int dist;

    QString info(){
        return QString("%1 - %2 МГц\nСредний RSSI %3 дБм").arg(freqTo2block(lower_freq).c_str()).arg(freqTo2block(upper_freq).c_str()).arg(rssi, 0, 'g', 3);
    }
};

struct MainWindow::Key{
    int crc = 0;
    QString values[8];
    int values2[8];
};

#endif // MAINWINDOW_H
