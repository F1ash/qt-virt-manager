#ifndef TCP_WIDGET_H
#define TCP_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class TcpWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit TcpWidget(QWidget *parent = 0, QString _tag = "");

signals:

private:
    QLabel          *modeLabel;
    QComboBox       *mode;
    QLabel          *hostLabel;
    QLineEdit       *host;
    QLabel          *portLabel;
    QSpinBox        *port;
    QLabel          *telnetLabel;
    QComboBox       *telnet;
    QGridLayout     *tcpLayout;
    QString          tag;

public slots:
    QDomDocument getDataDocument() const;
    void         setDataDescription(QString&);
};

#endif // TCP_WIDGET_H
