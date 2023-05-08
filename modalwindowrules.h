#ifndef MODALWINDOWRULES_H
#define MODALWINDOWRULES_H

#include <QMainWindow>
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

namespace Ui {
class ModalWindowRules;
}

class ModalWindowRules : public QMainWindow
{
    Q_OBJECT
    QString text;
    QMenu* toc;
    QAction *p1, *p2, *p3, *p4, *p5, *p6;

public:
    explicit ModalWindowRules(QWidget *parent = nullptr);
    ~ModalWindowRules();

private:
    Ui::ModalWindowRules *ui;
    inline void load_text();
    inline void set_toc();
    inline void _scroll(int pos);

};

#endif // MODALWINDOWRULES_H
