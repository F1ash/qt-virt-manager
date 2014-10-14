#ifndef VIRTNET_MODEL_H
#define VIRTNET_MODEL_H

#include "virt_objects/model.h"

class VirtNetModel : public Model
{
    Q_OBJECT
public:
    explicit VirtNetModel(QObject *parent = NULL);
};

#endif // VIRTNET_MODEL_H
