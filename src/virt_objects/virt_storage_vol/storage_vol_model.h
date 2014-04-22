#ifndef STORAGE_VOL_MODEL_H
#define STORAGE_VOL_MODEL_H

#include "virt_objects/model.h"

class StorageVolModel : public Model
{
    Q_OBJECT
public:
    explicit StorageVolModel(QObject *parent = 0);
};

#endif // STORAGE_VOL_MODEL_H
