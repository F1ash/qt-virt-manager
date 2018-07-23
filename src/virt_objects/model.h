#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QList>
#include "index.h"
//#include <QDebug>

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = Q_NULLPTR);
    ~Model();
    QList<Index *>  DataList;
    QIcon    activeIcon;
    QIcon    no_activeIcon;
    QIcon    defined;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRow(int);
    bool removeRow(int);

private:
    QString  column0;
    QString  column1;
    QString  column2;
    QString  column3;
};

#endif // MODEL_H
