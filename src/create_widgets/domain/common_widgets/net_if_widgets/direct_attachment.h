#ifndef DIRECT_ATTACHMENT_H
#define DIRECT_ATTACHMENT_H

#include "create_widgets/domain/_qwidget.h"
#include "virtual_port.h"
#include "create_widgets/domain/common_widgets/device_address.h"

class DirectAttachment : public _QWidget
{
    Q_OBJECT
public:
    explicit DirectAttachment(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QLabel              *netSourceLabel, *sourceModeLabel;
    QComboBox           *netSource, *sourceMode;
    QWidget             *baseWdg;
    QGridLayout         *baseLayout;
    VirtualPort         *virtPort;
    DeviceAddress       *addr;
    QVBoxLayout         *commonLayout;

public slots:
    QDomDocument         getDataDocument() const;
    void                 setDataDescription(QString&);

private slots:
    void                 setAvailableSources();
};

#endif // DIRECT_ATTACHMENT_H
