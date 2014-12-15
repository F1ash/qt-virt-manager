#ifndef HOSTDEV_MODE_WIDGET_H
#define HOSTDEV_MODE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class HOSTDEV_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit HOSTDEV_Mode_widget(QWidget *parent = 0);

signals:

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // HOSTDEV_MODE_WIDGET_H
