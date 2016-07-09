#include "bound_widget.h"

Bound_Widget::Bound_Widget(QWidget *parent) :
    QWidget(parent)
{
    averageL = new QCheckBox("A&verage (0-10000 KB/s)", this);
    averageL->setToolTip(
    "Specifies the desired average bit rate for the interface being shaped");
    peakL = new QCheckBox("&Peak (0-10000 KB/s)", this);
    peakL->setToolTip(
    "Specifies the maximum rate at which the bridge can send data");
    burstL = new QCheckBox("B&urst (0-10000 KB/s)", this);
    burstL->setToolTip(
    "Specifies the amount of kilobytes that can be transmitted\
 in a single burst at peak speed");
    floorL = new QCheckBox("F&loor (0-10000 KB/s)", this);
    floorL->setToolTip(
    "Guarantees minimal throughput for shaped interfaces");
    average = new QSpinBox(this);
    average->setRange(0, 10000);
    average->setEnabled(false);
    average->setAlignment(Qt::AlignRight);
    peak = new QSpinBox(this);
    peak->setRange(0, 10000);
    peak->setEnabled(false);
    peak->setAlignment(Qt::AlignRight);
    burst = new QSpinBox(this);
    burst->setRange(0, 10000);
    burst->setEnabled(false);
    burst->setAlignment(Qt::AlignRight);
    floor = new QSpinBox(this);
    floor->setRange(0, 10000);
    floor->setEnabled(false);
    floor->setAlignment(Qt::AlignRight);
    lt = new QGridLayout(this);
    lt->addWidget(averageL, 0, 0);
    lt->addWidget(average, 0, 1, Qt::AlignRight);
    lt->addWidget(peakL, 1, 0);
    lt->addWidget(peak, 1, 1, Qt::AlignRight);
    lt->addWidget(burstL, 2, 0);
    lt->addWidget(burst, 2, 1, Qt::AlignRight);
    lt->addWidget(floorL, 3, 0);
    lt->addWidget(floor, 3, 1, Qt::AlignRight);
    ltWdg = new QWidget(this);
    ltWdg->setLayout(lt);
    commonLayout = new QVBoxLayout();
    commonLayout->addWidget(ltWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);

    connect(averageL, SIGNAL(toggled(bool)),
            average, SLOT(setEnabled(bool)));
    connect(peakL, SIGNAL(toggled(bool)),
            peak, SLOT(setEnabled(bool)));
    connect(burstL, SIGNAL(toggled(bool)),
            burst, SLOT(setEnabled(bool)));
    connect(floorL, SIGNAL(toggled(bool)),
            floor, SLOT(setEnabled(bool)));
}

void Bound_Widget::setFloorUsage(bool state)
{
    floorL->setEnabled(state);
}
