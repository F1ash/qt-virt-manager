#ifndef HOSTDEV_MODE_WIDGET_H
#define HOSTDEV_MODE_WIDGET_H

#include "create_widgets/common/_qwidget.h"

class HOSTDEV_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit HOSTDEV_Mode_widget(QWidget *parent = Q_NULLPTR);

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;
};

#endif // HOSTDEV_MODE_WIDGET_H
