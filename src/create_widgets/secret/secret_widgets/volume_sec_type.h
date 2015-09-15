#ifndef VOLUME_SEC_TYPE_H
#define VOLUME_SEC_TYPE_H

#include "_sec_type.h"
#include <QPushButton>
#include <QLineEdit>
#include "create_widgets/domain/common_widgets/disk_widgets/virtvolume_dialog.h"

class VolumeSecType : public _SecType
{
    Q_OBJECT
public:
    explicit VolumeSecType(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

signals:

private:
    QLineEdit      *usage;
    QPushButton    *find;
    QHBoxLayout    *usageLayout;
    QWidget        *usageWdg;

public slots:
    QDomDocument    getSecStuff() const;
    void            showVolumeChoose();
};

#endif // VOLUME_SEC_TYPE_H
