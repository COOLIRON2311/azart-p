#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QListWidget"
#include <map>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public Q_SLOTS:

    void selfcontrol_screen();
    void loading_screen();
    void main_screen();
    void menu_screen();
    void service_menu_screen();
    void data_editor_screen();

    void channels_list_screen();
    void directions_list_screen();

    void channel_editor_screen();

private slots:
    void on_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_menu_list_itemSelectionChanged();

    void on_service_menu_list_itemSelectionChanged();

    void on_service_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_data_editor_list_itemDoubleClicked(QListWidgetItem *item);

    void on_service_menu_back_clicked();

    void on_channels_list_back_clicked();

    void on_directions_list_back_clicked();

    void on_data_editor_back_clicked();

    void on_channel_menu_button_clicked();

    void on_channel_popup_menu_list_itemDoubleClicked(QListWidgetItem *item);

    void on_channel_editor_back_clicked();

    void on_pushButton_clicked();

    void on_channel_editor_state_currentIndexChanged(int index);

    void on_channels_list_itemClicked(QListWidgetItem *item);

private:
    struct Channel;
    struct Direction;

    Ui::MainWindow *ui;

    std::map<QString, QListWidgetItem*> selected_items;

    QListWidgetItem* menu_list_item[7];
    QListWidgetItem* service_menu_list_item[9];
    QListWidgetItem* data_editor_list_item[9];

    std::vector<Direction*> directions_list_item;
    std::map<QListWidgetItem*, Channel*> channels_map;
    int current_direction = -1;
    int current_channel = -1;


    QListWidgetItem* channels_popup_menu_list_item[3];
};

struct MainWindow::Channel
{
    quint32 state = 0; //"Не задано"
    bool PRD = false;
    bool dualfreq = false;
    quint32 freq = 0;
    quint32 ctcss = 0;
    QString name = "";
};

struct MainWindow::Direction
{
    Channel* ch;
};

#endif // MAINWINDOW_H
