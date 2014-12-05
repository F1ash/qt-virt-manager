#ifndef FORWARD_WIDGET_H
#define FORWARD_WIDGET_H

#include "create_widgets/domain/_qwidget.h"

class Forward_Widget : public _QWidget
{
    Q_OBJECT
public:
    explicit Forward_Widget(QWidget *parent = 0);
    QComboBox       *mode;

signals:

private:
    QCheckBox       *title;
    QLabel          *modeLabel;
    QGridLayout     *frwdLayout;
    QWidget         *forwards;
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // FORWARD_WIDGET_H
