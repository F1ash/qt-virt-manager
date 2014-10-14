#ifndef STORAGE_VOL_MODEL_H
#define STORAGE_VOL_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QDebug>
#include "storage_vol_index.h"

class StorageVolModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StorageVolModel(QObject *parent = NULL);
    ~StorageVolModel();
    QList<StorageVolIndex *>  DataList;
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
    QString  column2;
    QString  column3;
    QString  column4;

public slots:
};

#endif // STORAGE_VOL_MODEL_H
