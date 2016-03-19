#ifndef STORAGE_POOL_LIST_DELEGATE_H
#define STORAGE_POOL_LIST_DELEGATE_H

#include <QStyledItemDelegate>

class StoragePoolListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit StoragePoolListDelegate(QObject *parent = nullptr);

private slots:
    void paint(
            QPainter *painter,
            const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
};

#endif // STORAGE_POOL_LIST_DELEGATE_H
