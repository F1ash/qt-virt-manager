#include "storage_vol_model.h"

StorageVolModel::StorageVolModel(QObject *parent) :
    Model(parent)
{
    defined = QIcon::fromTheme("storageVol-define");
    created = QIcon::fromTheme("storageVol-create");
}
