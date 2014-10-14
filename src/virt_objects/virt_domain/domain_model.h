#ifndef DOMAIN_MODEL_H
#define DOMAIN_MODEL_H

#include "virt_objects/model.h"

class DomainModel : public Model
{
    Q_OBJECT
public:
    explicit DomainModel(QObject *parent = NULL);
    QVariant data(const QModelIndex&, int) const;
};

#endif // DOMAIN_MODEL_H
