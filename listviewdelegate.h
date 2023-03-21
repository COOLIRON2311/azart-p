#ifndef LISTVIEWDELEGATE_H
#define LISTVIEWDELEGATE_H

#include <QtWidgets/QApplication>
#include <QtGui>
#include <QColor>
#include <QStyledItemDelegate>

class ListWidgetItemDelegate: public QStyledItemDelegate
{
    double w;
    QBrush b;
    QString indent;
public:
   ListWidgetItemDelegate(double width = 0.05, int indent = 2, QObject* parent = nullptr) : QStyledItemDelegate(parent)
   {
       this->indent = QString(" ").repeated(indent);
       b.setColor(QColor("#7597D9"));
       b.setStyle(Qt::Dense6Pattern);
       w = 1 - width;
   }

    void paint(QPainter* p, const QStyleOptionViewItem& o, const QModelIndex& idx) const override
    {
        p->setPen(Qt::NoPen);
        p->setBrush(b);
        auto r = o.rect.adjusted(0, 0, -o.rect.width()*w, 0);
        p->drawPolygon(r);
        QStyledItemDelegate::paint(p, o, idx);
    }
    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        Q_UNUSED(locale)
        QString result = indent + value.toString();
        return result;
    }
};

#endif // LISTVIEWDELEGATE_H
