#ifndef CHAR_DEVICE_H
#define CHAR_DEVICE_H

#include "pty_widget.h"
#include "dev_widget.h"
#include "file_widget.h"
#include "tcp_widget.h"
#include "udp_widget.h"
#include "unix_widget.h"

class CharDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit CharDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL
            );
    ~CharDevice();
    QComboBox       *devType;
    PtyWidget       *ptyWdg;
    DevWidget       *devWdg;
    FileWidget      *fileWdg;
    TcpWidget       *tcpWdg;
    UdpWidget       *udpWdg;
    UnixWidget      *unixWdg;
    QVBoxLayout     *commonLayout;

signals:

private:

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;

private slots:
    virtual void devTypeChanged(int);
};

#endif // CHAR_DEVICE_H
