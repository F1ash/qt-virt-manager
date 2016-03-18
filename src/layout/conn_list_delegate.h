#ifndef CONN_LIST_DELEGATE_H
#define CONN_LIST_DELEGATE_H

#include <QStyledItemDelegate>
#include <QApplication>

class ConnListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ConnListDelegate(QObject *parent = nullptr);

signals:

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;

};

#endif // CONN_LIST_DELEGATE_H
