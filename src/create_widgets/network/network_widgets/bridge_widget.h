#ifndef BRIDGE_WIDGET_H
#define BRIDGE_WIDGET_H

#include "_checked_widget.h"

class Bridge_Widget : public _Checked_Widget
{
    Q_OBJECT
public:
    explicit Bridge_Widget(
            QWidget *parent = nullptr,
            QString  tag = "Bridge");
    QLineEdit       *bridgeName;
    QCheckBox       *stp;
    QSpinBox        *delay;
    QComboBox       *macTableManager;

private:
    QLabel          *delayLabel,* macTabLabel;
    QHBoxLayout     *bridgeLt, *stpLt, *macTabLt;
    QWidget         *bridgeWdg, *stpWdg, *macTabWdg;
};

#endif // BRIDGE_WIDGET_H
