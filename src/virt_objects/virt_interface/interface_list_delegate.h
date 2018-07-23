#ifndef INTERFACE_LIST_DELEGATE_H
#define INTERFACE_LIST_DELEGATE_H

#include <QStyledItemDelegate>

class IfaceListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit IfaceListDelegate(QObject *parent = Q_NULLPTR);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // INTERFACE_LIST_DELEGATE_H
