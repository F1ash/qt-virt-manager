#ifndef PASSTHROUGH_MODE_WIDGET_H
#define PASSTHROUGH_MODE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class PASSTHROUGH_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit PASSTHROUGH_Mode_widget(QWidget *parent = 0);

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;
};

#endif // PASSTHROUGH_MODE_WIDGET_H
