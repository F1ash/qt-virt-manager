#ifndef VEPA_MODE_WIDGET_H
#define VEPA_MODE_WIDGET_H

#include "create_widgets/common/_qwidget.h"

class VEPA_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit VEPA_Mode_widget(QWidget *parent = Q_NULLPTR);

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;
};

#endif // VEPA_MODE_WIDGET_H
