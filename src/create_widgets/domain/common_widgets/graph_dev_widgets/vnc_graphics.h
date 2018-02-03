#ifndef VNC_GRAPHICS_H
#define VNC_GRAPHICS_H

#include "create_widgets/common/_qwidget.h"
#include "virt_objects/_virt_thread.h"

class vnc_graphHlpThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit vnc_graphHlpThread(
            QObject        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);
    void             run();
signals:
    void             result(QStringList&);
};

class VNC_Graphics : public _QWidget
{
    Q_OBJECT
public:
    explicit VNC_Graphics(
            QWidget        *parent      = nullptr,
            virConnectPtr*  connPtrPtr  = nullptr);

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
    void             setDataDescription(const QString&);

private slots:
    void             usePort(bool);
    void             usePassword(bool);
    void             addressEdit(int);
    void             readNetworkList(QStringList&);
    void             emitCompleteSignal();
};

#endif // VNC_GRAPHICS_H
