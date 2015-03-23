#ifndef JOBITEM_MODEL_H
#define JOBITEM_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QMap>
#include "conn_item_index.h"
#include <QDebug>

class ConnItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ConnItemModel(QObject *parent = NULL);
    ~ConnItemModel();
    QList<ConnItemIndex *>  connItemDataList;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

signals:

private:
    QModelIndex  rootIdx;
    QIcon        onViewIcon;
    QIcon        activeIcon;
    QIcon        no_activeIcon;
    QMap<QString, QIcon>
                 uri_logo;

};

#endif // JOBITEM_MODEL_H
