#ifndef MODALWINDOWRULES_H
#define MODALWINDOWRULES_H

#include <QMainWindow>
#include <QFile>

namespace Ui {
class ModalWindowRules;
}

class ModalWindowRules : public QMainWindow
{
    Q_OBJECT
    QString text;

public:
    explicit ModalWindowRules(QWidget *parent = nullptr);
    ~ModalWindowRules();

private:
    Ui::ModalWindowRules *ui;
    void load_text();
};

#endif // MODALWINDOWRULES_H
