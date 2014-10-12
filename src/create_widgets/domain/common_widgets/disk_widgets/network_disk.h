#ifndef NETWORK_DISK_H
#define NETWORK_DISK_H

#include "_disk.h"

class Network_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Network_Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *protocolLabel, *sourceNameLabel;
    QComboBox       *protocol;
    QLineEdit       *sourceName;

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void protocolTypeChanged(int);
};

#endif // NETWORK_DISK_H
