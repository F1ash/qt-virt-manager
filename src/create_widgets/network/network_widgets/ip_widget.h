#ifndef IP_WIDGET_H
#define IP_WIDGET_H

#include "_checked_widget.h"

class IP_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    IP_Widget(
            QWidget *parent = NULL,
            QString tag = "IP");

private:

public slots:
    QDomDocument         getDataDocument() const;
};

#endif // IP_WIDGET_H
