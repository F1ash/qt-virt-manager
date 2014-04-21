#ifndef STORAGE_VOL_MODEL_H
#define STORAGE_VOL_MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include <QDebug>
#include "virt_objects/index.h"

class StorageVolModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StorageVolModel(QObject *parent = 0);
    ~StorageVolModel();
    QList<Index *>  virtStorageVolDataList;

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
    QIcon    activeIcon;
    QIcon    no_activeIcon;
    QIcon    defined;
    QIcon    created;

public slots:
};

#endif // STORAGE_VOL_MODEL_H
