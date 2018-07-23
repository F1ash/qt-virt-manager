#ifndef DISK_EDIT_H
#define DISK_EDIT_H

#include "create_widgets/domain/common_widgets/disk.h"

class Disk_Edit : public Disk
{
    Q_OBJECT
public:
    explicit Disk_Edit(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr*  connPtrPtr = Q_NULLPTR);

signals:

public slots:
    void         setDataDescription(const QString&);
};

#endif // DISK_EDIT_H
