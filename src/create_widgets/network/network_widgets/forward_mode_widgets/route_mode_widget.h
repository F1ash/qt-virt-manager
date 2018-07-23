#ifndef ROUTE_MODE_WIDGET_H
#define ROUTE_MODE_WIDGET_H

#include "create_widgets/common/_qwidget.h"

class ROUTE_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit ROUTE_Mode_widget(QWidget *parent = Q_NULLPTR);

private:
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
};

#endif // ROUTE_MODE_WIDGET_H
