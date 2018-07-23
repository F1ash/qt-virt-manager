#ifndef NET_LIST_DELEGATE_H
#define NET_LIST_DELEGATE_H

#include <QStyledItemDelegate>

class NetworkListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NetworkListDelegate(QObject *parent = Q_NULLPTR);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // NET_LIST_DELEGATE_H
