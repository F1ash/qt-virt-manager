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
    // for Network Assistant
    void             disableOptionalAttr(bool);
    bool             bridgeNameIsEmpty() const;

private:
    QLineEdit       *bridgeName;
    QCheckBox       *stp;
    QSpinBox        *delay;
    QComboBox       *macTableManager;
    QLabel          *delayLabel,* macTabLabel;
    QHBoxLayout     *bridgeLt, *macTabLt;
    QWidget         *bridgeWdg, *macTabWdg;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // BRIDGE_WIDGET_H
