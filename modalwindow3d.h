#ifndef MODALWINDOW3D_H
#define MODALWINDOW3D_H

#include <QMainWindow>

namespace Ui {
class ModalWindow3D;
}

class ModalWindow3D : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModalWindow3D(QWidget *parent = nullptr);
    ~ModalWindow3D();

private:
    Ui::ModalWindow3D *ui;
};

#endif // MODALWINDOW3D_H
