#ifndef NET_LIST_DELEGATE_H
#define NET_LIST_DELEGATE_H

#include <QStyledItemDelegate>

class NetworkListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit NetworkListDelegate(QObject *parent = nullptr);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // NET_LIST_DELEGATE_H
