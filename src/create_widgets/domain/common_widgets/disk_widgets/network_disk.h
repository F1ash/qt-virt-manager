#ifndef NETWORK_DISK_H
#define NETWORK_DISK_H

#include "_disk.h"
#include "virtvolume_dialog.h"
#include "create_widgets/storage/_create_storage_widgets/_auth.h"

class Network_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Network_Disk(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtr = NULL);

private:
    QLabel          *protocolLabel, *authLabel;
    QComboBox       *protocol;
    QPushButton     *sourceLabel;
    QLineEdit       *sourceName;
    VirtVolumeDialog
                    *volumeDialog = NULL;
    _Storage_Auth   *auth;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             protocolTypeChanged(int);
    void             protocolTypeChanged(QString);
    void             getVolumeNames();
    void             setTypedData(VVD_Result&);
};

#endif // NETWORK_DISK_H
