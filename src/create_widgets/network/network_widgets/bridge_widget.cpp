#include "bridge_widget.h"

Bridge_Widget::Bridge_Widget(QWidget *parent) :
    _QWidget(parent)
{
    title = new QCheckBox("Bridge", this);
    bridgeName = new QLineEdit(this);
    bridgeName->setPlaceholderText("Enter bridge name (Ex.: virbr1)");
    bridgeLt = new QHBoxLayout(this);
    bridgeLt->addWidget(bridgeName);
    stp = new QCheckBox("Spanning Tree Protocol", this);
    stp->setChecked(true); // default state
    delayLabel = new QLabel("Delay", this);
    delay = new QSpinBox(this);
    delay->setRange(0, 360);
    stpLt = new QHBoxLayout();
    stpLt->addWidget(stp);
    stpLt->addSpacing(25);
    stpLt->addWidget(delayLabel);
    stpLt->addWidget(delay);
    macTabLabel = new QLabel("macTableManager", this);
    macTableManager = new QComboBox(this);
    macTableManager->addItems(QStringList()<<"kernel"<<"libvirt");
    macTabLt = new QHBoxLayout();
    macTabLt->addWidget(macTabLabel);
    macTabLt->addWidget(macTableManager);
    bridgeWdg = new QWidget(this);
    bridgeWdg->setLayout(bridgeLt);
    bridgeWdg->setEnabled(false);
    stpWdg = new QWidget(this);
    stpWdg->setLayout(stpLt);
    stpWdg->setEnabled(false);
    macTabWdg = new QWidget(this);
    macTabWdg->setLayout(macTabLt);
    macTabWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(title);
    commonLayout->addWidget(bridgeWdg);
    commonLayout->addWidget(stpWdg);
    commonLayout->addWidget(macTabWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(title, SIGNAL(toggled(bool)),
            bridgeWdg, SLOT(setEnabled(bool)));
    connect(title, SIGNAL(toggled(bool)),
            stpWdg, SLOT(setEnabled(bool)));
    connect(title, SIGNAL(toggled(bool)),
            macTabWdg, SLOT(setEnabled(bool)));
}
