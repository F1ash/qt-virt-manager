#ifndef STORAGE_POOL_MODEL_H
#define STORAGE_POOL_MODEL_H

#include "virt_objects/model.h"

class StoragePoolModel : public Model
{
    Q_OBJECT
public:
    explicit StoragePoolModel(QObject *parent = NULL);
};

#endif // STORAGE_POOL_MODEL_H
