#ifndef BRIDGE_MODE_WIDGET_H
#define BRIDGE_MODE_WIDGET_H

#include "create_widgets/common/_qwidget.h"

class BRIDGE_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit BRIDGE_Mode_widget(QWidget *parent = Q_NULLPTR);

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;
};

#endif // BRIDGE_MODE_WIDGET_H
