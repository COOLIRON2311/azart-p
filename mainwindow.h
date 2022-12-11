#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTimer"
#include "QListWidget"

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

    void on_comboBox_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QListWidgetItem* selected_item = NULL;

    QListWidgetItem* menu_list_item[7];
    QListWidgetItem* service_menu_list_item[9];
    QListWidgetItem* data_editor_list_item[9];

    std::vector<QListWidgetItem*> directions_list_item;
    std::vector<QListWidgetItem*> channels_list_item;

    QListWidgetItem* channels_popup_menu_list_item[3];
};

#endif // MAINWINDOW_H
