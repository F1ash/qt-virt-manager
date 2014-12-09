#ifndef FORWARD_WIDGET_H
#define FORWARD_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include "nat_mode_widget.h"
#include "route_mode_widget.h"
#include "bridge_mode_widget.h"
#include "private_mode_widget.h"
#include "vepa_mode_widget.h"
#include "passthrough_mode_widget.h"
#include "hostdev_mode_widget.h"
#include <QStackedWidget>

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
    QStackedWidget  *frwdModeSet;
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // FORWARD_WIDGET_H
