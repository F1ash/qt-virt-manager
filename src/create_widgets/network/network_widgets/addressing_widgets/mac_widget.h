#ifndef MAC_WIDGET_H
#define MAC_WIDGET_H

#include "create_widgets/common/_checked_widget.h"

class MAC_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    MAC_Widget(
            QWidget *parent = nullptr,
            QString  tag = "MAC");

private:
    QLineEdit       *mac;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // MAC_WIDGET_H
