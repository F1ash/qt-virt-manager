#ifndef HOST_DNS_H
#define HOST_DNS_H

#include "create_widgets/common/_list_widget.h"

class Host_DNS : public _List_Widget
{
    Q_OBJECT
public:
    Host_DNS(
            QWidget *parent = nullptr,
            QString  tag    = "HOSTs");

private:
    QLabel          *ipLabel;
    QHBoxLayout     *ipLayout;
    QWidget         *ipWidget;
    QLineEdit       *hostIP, *hostName;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
};

#endif // HOST_DNS_H
