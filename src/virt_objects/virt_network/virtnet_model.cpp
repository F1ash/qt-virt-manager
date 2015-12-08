#include "virtnet_model.h"

VirtNetModel::VirtNetModel(QObject *parent) :
    Model(parent)
{
    defined = QIcon::fromTheme("network");
    //created = QIcon::fromTheme("network-create");
}
