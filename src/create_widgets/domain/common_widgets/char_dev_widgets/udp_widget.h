#ifndef UDP_WIDGET_H
#define UDP_WIDGET_H

#include "create_widgets/common/_qwidget.h"

class UdpWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit UdpWidget(
            QWidget *parent = nullptr,
            QString  _tag   = "");

private:
    QLabel          *hostLabel;
    QLineEdit       *host;
    QLabel          *port1Label;
    QSpinBox        *port1;
    QLabel          *bindHostLabel;
    QLineEdit       *bindHost;
    QLabel          *port2Label;
    QSpinBox        *port2;
    QGridLayout     *udpLayout;
    QString          tag;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // UDP_WIDGET_H
