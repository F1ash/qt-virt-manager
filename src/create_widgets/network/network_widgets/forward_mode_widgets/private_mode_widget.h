#ifndef PRIVATE_MODE_WIDGET_H
#define PRIVATE_MODE_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class PRIVATE_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit PRIVATE_Mode_widget(QWidget *parent = 0);

signals:

private:
    QLabel          *info;
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // PRIVATE_MODE_WIDGET_H
