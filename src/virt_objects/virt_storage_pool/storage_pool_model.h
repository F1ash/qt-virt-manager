#ifndef STORAGE_POOL_MODEL_H
#define STORAGE_POOL_MODEL_H

#include "virt_objects/model.h"

class StoragePoolModel : public Model
{
    Q_OBJECT
public:
    explicit StoragePoolModel(QObject *parent = nullptr);
    //QIcon    onViewIcon;
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // STORAGE_POOL_MODEL_H
