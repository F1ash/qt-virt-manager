#ifndef IFACE_MODEL_H
#define IFACE_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include "virt_interface_index.h"
#include <QDebug>

class VirtIfaceModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VirtIfaceModel(QObject *parent = nullptr);
    ~VirtIfaceModel();
    QList<Interface_Index *>  DataList;
    QIcon    activeIcon;
    QIcon    no_activeIcon;
    QIcon    defined;
    QIcon    changing;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

private:
    QString  column0;
    QString  column1;
    QString  column2;
    QString  column3;
};

#endif // IFACE_MODEL_H
