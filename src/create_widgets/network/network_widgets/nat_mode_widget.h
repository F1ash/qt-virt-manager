#ifndef NAT_MODE_WIDGET_H
#define NAT_MODE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class NAT_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit NAT_Mode_widget(QWidget *parent = 0);
    QLineEdit       *dev, *ipStart, *ipEnd;
    QSpinBox        *portStart, *portEnd;
    QCheckBox       *addrRange, *portRange;

private:
    QLabel          *devLabel;
    QHBoxLayout     *devLayout, *addrLayout, *portLayout;
    QWidget         *devWdg, *addrWdg, *portWdg;
    QVBoxLayout     *commonLayout;

private slots:
    void             portStartChanged(int);
    void             portEndChanged(int);
};

#endif // NAT_MODE_WIDGET_H
