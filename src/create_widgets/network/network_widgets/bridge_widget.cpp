#include "bridge_widget.h"

Bridge_Widget::Bridge_Widget(QWidget *parent) :
    _QWidget(parent)
{
    title = new QCheckBox("Bridge", this);
    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter bridge name (Ex.: virbr1)");
    stp = new QCheckBox("Spanning Tree Protocol", this);
    stp->setChecked(true); // default state
    delayLabel = new QLabel("Delay", this);
    delay = new QSpinBox(this);
    delay->setRange(0, 360);
    bridgeLayout = new QGridLayout();
    bridgeLayout->addWidget(bridgeName, 0, 0, 1, 2);
    bridgeLayout->addWidget(stp, 2, 0, 3, 2);
    bridgeLayout->addWidget(delayLabel, 4, 0, 5, 1);
    bridgeLayout->addWidget(delay, 4, 1, 5, 2);
    bridge = new QWidget(this);
    bridge->setLayout(bridgeLayout);
    bridge->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(title);
    commonLayout->addWidget(bridge);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(title, SIGNAL(toggled(bool)),
            bridge, SLOT(setEnabled(bool)));
}
