#ifndef DISK_EDIT_H
#define DISK_EDIT_H

#include "create_widgets/domain/common_widgets/disk.h"

class Disk_Edit : public Disk
{
    Q_OBJECT
public:
    explicit Disk_Edit(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

signals:

public slots:
    void         setDataDescription(QString&);
};

#endif // DISK_EDIT_H
