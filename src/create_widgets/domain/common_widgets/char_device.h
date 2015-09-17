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
            QWidget        *parent      = NULL,
            virConnectPtr*  connPtrPtr  = NULL,
            virDomainPtr    domain      = NULL,
            QString         _tag        = QString());

    QComboBox       *devType;
    QStackedWidget  *charDevWdg;
    QVBoxLayout     *commonLayout;
    QString          tag;

private:
    PtyWidget       *ptyWdg;
    DevWidget       *devWdg;
    FileWidget      *fileWdg;
    TcpWidget       *tcpWdg;
    UdpWidget       *udpWdg;
    UnixWidget      *unixWdg;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // CHAR_DEVICE_H
