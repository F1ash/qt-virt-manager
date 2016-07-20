#ifndef DIRECT_ATTACHMENT_H
#define DIRECT_ATTACHMENT_H

#include "create_widgets/domain/_qwidget.h"
#include "virtual_port.h"
#include "create_widgets/domain/common_widgets/device_address.h"
#include "virt_objects/_virt_thread.h"

class dirAttach_HlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit dirAttach_HlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    void             run();
signals:
    void             result(QStringList&);
};

class DirectAttachment : public _QWidget
{
    Q_OBJECT
public:
    explicit DirectAttachment(
            QWidget        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr);

private:
    QLabel          *netSourceLabel, *sourceModeLabel;
    QComboBox       *netSource, *sourceMode;
    QWidget         *baseWdg;
    QGridLayout     *baseLayout;
    VirtualPort     *virtPort;
    DeviceAddress   *addr;
    QVBoxLayout     *commonLayout;

    dirAttach_HlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             setAvailableSources(QStringList&);
    void             emitCompleteSignal();
};

#endif // DIRECT_ATTACHMENT_H
