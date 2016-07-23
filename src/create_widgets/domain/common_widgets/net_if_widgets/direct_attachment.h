#ifndef DIRECT_ATTACHMENT_H
#define DIRECT_ATTACHMENT_H

#include "create_widgets/common/_qwidget_threaded.h"
#include "virtual_port.h"
#include "nwfilter_parameters.h"
#include "create_widgets/domain/device_address.h"

class dirAttach_HlpThread : public qwdHelpThread
{
    Q_OBJECT
public:
    explicit dirAttach_HlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    QStringList      nwFilters;
    void             run();
signals:
    void             result(QStringList&);
};

class DirectAttachment : public _QWidget_Threaded
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
    NWFilter_Params *nwFilterParams;
    QVBoxLayout     *commonLayout;

    dirAttach_HlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             sourceModeChanged(QString);
    void             setAvailableSources(QStringList&);
    void             emitCompleteSignal();
};

#endif // DIRECT_ATTACHMENT_H
