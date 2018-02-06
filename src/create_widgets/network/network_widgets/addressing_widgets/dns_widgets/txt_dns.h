#ifndef TXT_DNS_H
#define TXT_DNS_H

#include "create_widgets/common/_list_widget.h"

class TXT_DNS : public _List_Widget
{
    Q_OBJECT
public:
    explicit TXT_DNS(
            QWidget *parent = nullptr,
            QString  tag    = "TXTs");

private:
    QLabel          *comma;
    QLineEdit       *hostValue, *hostName;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
    void             addItem();
};

#endif // TXT_DNS_H
