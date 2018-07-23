#ifndef SNAPSHOT_TREE_MODEL_H
#define SNAPSHOT_TREE_MODEL_H

#include <QAbstractItemModel>
#include <QIcon>
#include <QList>
#include "snapshot_tree_index.h"
//#include <QDebug>

class SnapshotTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SnapshotTreeModel(QObject *parent = Q_NULLPTR);
    QIcon                        icon, work;

    QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int, const QModelIndex&);
    bool removeRow(int, const QModelIndex&);

signals:

private:
    TreeItem *rootItem;

public slots:
    int       rootItemChildCount() const;
};

#endif // SNAPSHOT_TREE_MODEL_H
