#ifndef CHAR_DEVICE_H
#define CHAR_DEVICE_H

#include "pty_widget.h"
#include "dev_widget.h"
#include "file_widget.h"
#include "tcp_widget.h"
#include "udp_widget.h"
#include "unix_widget.h"

#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class CharDevice : public _QWidget
{
    Q_OBJECT
public:
    explicit CharDevice(QWidget *parent = 0);
    ~CharDevice();

signals:

private:
    QComboBox       *devType;
    PtyWidget       *ptyWdg;
    DevWidget       *devWdg;
    FileWidget      *fileWdg;
    TcpWidget       *tcpWdg;
    UdpWidget       *udpWdg;
    UnixWidget      *unixWdg;

    QVBoxLayout     *commonLayout;

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;

private slots:
    virtual void devTypeChanged(int);
};

#endif // CHAR_DEVICE_H
