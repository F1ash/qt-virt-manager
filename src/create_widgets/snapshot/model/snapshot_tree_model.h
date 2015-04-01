#ifndef SNAPSHOT_TREE_MODEL_H
#define SNAPSHOT_TREE_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QDebug>
#include "snapshot_tree_index.h"

class SnapshotTreeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SnapshotTreeModel(QObject *parent = NULL);
    QList<SnapshotTreeIndex *>   DataList;
    QIcon                        icon;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int, const QModelIndex&);
    bool removeRow(int, const QModelIndex&);

signals:

private:
    QString  column0;

public slots:
};

#endif // SNAPSHOT_TREE_MODEL_H
