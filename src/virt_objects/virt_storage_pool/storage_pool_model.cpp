#include "storage_pool_model.h"

StoragePoolModel::StoragePoolModel(QObject *parent) :
    Model(parent)
{
    defined = QIcon::fromTheme("storagePool-define");
    created = QIcon::fromTheme("storagePool-create");
}
