#ifndef CHAR_DEVICE_H
#define CHAR_DEVICE_H

#include <QStackedWidget>
#include "char_dev_widgets/pty_widget.h"
#include "char_dev_widgets/dev_widget.h"
#include "char_dev_widgets/file_widget.h"
#include "char_dev_widgets/tcp_widget.h"
#include "char_dev_widgets/udp_widget.h"
#include "char_dev_widgets/unix_widget.h"

class CharDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit CharDevice(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            virDomainPtr domain = NULL,
            QString _tag = QString()
            );

    QComboBox       *devType;
    QStackedWidget  *charDevWdg;
    QVBoxLayout     *commonLayout;

signals:

private:
    PtyWidget       *ptyWdg;
    DevWidget       *devWdg;
    FileWidget      *fileWdg;
    TcpWidget       *tcpWdg;
    UdpWidget       *udpWdg;
    UnixWidget      *unixWdg;
    QString          tag;

public slots:
    QDomDocument     getDevDocument() const;
    void             setDeviceData(QString&);
};

#endif // CHAR_DEVICE_H
