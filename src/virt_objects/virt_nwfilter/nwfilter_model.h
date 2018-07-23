#ifndef VIRT_NWFILTER_MODEL_H
#define VIRT_NWFILTER_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include "nwfilter_index.h"
//#include <QDebug>

class VirtNWFilterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VirtNWFilterModel(QObject *parent = Q_NULLPTR);
    ~VirtNWFilterModel();
    QList<NWFilter_Index *>  DataList;
    QIcon    defined;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

signals:

private:
    QString  column0;
    QString  column1;
};

#endif // VIRT_NWFILTER_MODEL_H
