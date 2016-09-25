#ifndef DOMAIN_MODEL_H
#define DOMAIN_MODEL_H

#include "virt_objects/model.h"

class DomainModel : public Model
{
    Q_OBJECT
public:
    explicit DomainModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex&, int) const;
    bool     setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // DOMAIN_MODEL_H
