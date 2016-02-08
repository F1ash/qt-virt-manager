#ifndef VOLUME_DISK_H
#define VOLUME_DISK_H

#include <QPushButton>
#include "_disk.h"
#include "virtvolume_dialog.h"
#include "create_widgets/storage/_create_storage_widgets/_auth.h"

class Volume_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Volume_Disk(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

private:
    QLabel          *poolLabel, *modeLabel,
                    *pool;
    QPushButton     *volumeLabel;
    QLineEdit       *volume;
    QComboBox       *mode;
    _Storage_Auth   *auth;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             getVolumeNames();
};

#endif // VOLUME_DISK_H
