#ifndef MODALWINDOWRD_H
#define MODALWINDOWRD_H

#include <QMainWindow>

namespace Ui {
class ModalWindowRD;
}

class ModalWindowRD : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModalWindowRD(QWidget *parent = nullptr);
    ~ModalWindowRD();

private:
    Ui::ModalWindowRD *ui;
};

#endif // MODALWINDOWRD_H
