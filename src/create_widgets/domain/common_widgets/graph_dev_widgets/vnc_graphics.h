#ifndef VNC_GRAPHICS_H
#define VNC_GRAPHICS_H

#include "create_widgets/domain/_qwidget.h"
#include "virt_objects/_virt_thread.h"

class vnc_graphHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit vnc_graphHlpThread(
            QObject        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL);
    void             run();
signals:
    void             result(QStringList&);
};

class VNC_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit VNC_Graphics(
            QWidget        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL);

private:
    QLabel          *addrLabel;
    QComboBox       *address;
    QComboBox       *networks;
    QCheckBox       *autoPort;
    QSpinBox        *port;
    QCheckBox       *usePassw;
    QLineEdit       *passw;
    QLabel          *keymapLabel;
    QComboBox       *keymap;
    QLabel          *shareLabel;
    QComboBox       *sharePolicy;
    QGridLayout     *commonLayout;

    QStringList      nets;
    vnc_graphHlpThread
                    *hlpThread;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             usePort(bool);
    void             usePassword(bool);
    void             addressEdit(QString);
    void             readNetworkList(QStringList&);
};

#endif // VNC_GRAPHICS_H
