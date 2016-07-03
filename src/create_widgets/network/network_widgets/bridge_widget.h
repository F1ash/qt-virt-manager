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

private:
    QLineEdit       *bridgeName;
    QCheckBox       *stp;
    QSpinBox        *delay;
    QComboBox       *macTableManager;
    QLabel          *delayLabel,* macTabLabel;
    QHBoxLayout     *bridgeLt, *stpLt, *macTabLt;
    QWidget         *bridgeWdg, *stpWdg, *macTabWdg;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // BRIDGE_WIDGET_H
