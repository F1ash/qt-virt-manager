#include "forward_widget.h"

#define FORWARD_MODE QStringList()\
    <<"nat"\
    <<"route"\
    <<"bridge"\
    <<"private"\
    <<"vepa"\
    <<"passthrough"\
    <<"hostdev"

Forward_Widget::Forward_Widget(QWidget *parent) :
    _QWidget(parent)
{
    title = new QCheckBox("Forwards", this);
    modeLabel = new QLabel("Mode:", this);
    mode = new QComboBox(this);
    mode->addItems(FORWARD_MODE);
    frwdLayout = new QGridLayout();
    frwdLayout->addWidget(modeLabel, 0, 0);
    frwdLayout->addWidget(mode, 0, 1);
    forwards = new QWidget(this);
    forwards->setLayout(frwdLayout);
    forwards->setEnabled(false);
    frwdModeSet = new QStackedWidget(this);
    frwdModeSet->addWidget(new NAT_Mode_widget(this));
    frwdModeSet->addWidget(new ROUTE_Mode_widget(this));
    frwdModeSet->addWidget(new BRIDGE_Mode_widget(this));
    frwdModeSet->addWidget(new PRIVATE_Mode_widget(this));
    frwdModeSet->addWidget(new VEPA_Mode_widget(this));
    frwdModeSet->addWidget(new PASSTHROUGH_Mode_widget(this));
    frwdModeSet->addWidget(new HOSTDEV_Mode_widget(this));
    frwdModeSet->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(title);
    commonLayout->addWidget(forwards);
    commonLayout->addWidget(frwdModeSet);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(title, SIGNAL(toggled(bool)),
            forwards, SLOT(setEnabled(bool)));
    connect(title, SIGNAL(toggled(bool)),
            frwdModeSet, SLOT(setEnabled(bool)));
    connect(mode, SIGNAL(currentIndexChanged(int)),
            frwdModeSet, SLOT(setCurrentIndex(int)));
    connect(mode, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(modeChanged(const QString&)));
}

/* private slots */
void Forward_Widget::modeChanged(const QString &_mode)
{
    emit optionalsNeed( _mode=="nat" || _mode=="route" );
}
