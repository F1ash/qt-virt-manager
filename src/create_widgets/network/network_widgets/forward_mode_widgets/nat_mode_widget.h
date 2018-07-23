#ifndef NAT_MODE_WIDGET_H
#define NAT_MODE_WIDGET_H

#include "create_widgets/common/_qwidget.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

class NAT_Mode_widget : public _QWidget
{
    Q_OBJECT
public:
    explicit NAT_Mode_widget(QWidget *parent = Q_NULLPTR);

private:
    QLineEdit       *ipStart, *ipEnd;
    QSpinBox        *portStart, *portEnd;
    QCheckBox       *addrRange, *portRange;
    QHBoxLayout     *addrLayout, *portLayout;
    QWidget         *addrWdg, *portWdg;
    QVBoxLayout     *commonLayout;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             portStartChanged(int);
    void             portEndChanged(int);
};

#endif // NAT_MODE_WIDGET_H
