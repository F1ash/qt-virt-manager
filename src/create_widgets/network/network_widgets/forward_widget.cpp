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
    devLabel = new QCheckBox("Dev name:", this);
    dev = new QLineEdit(this);
    dev->setPlaceholderText("eth0 | enp2s0 | wlp3s0");
    devLayout = new QHBoxLayout(this);
    devLayout->addWidget(devLabel);
    devLayout->addWidget(dev);
    devWdg = new QWidget(this);
    devWdg->setLayout(devLayout);
    frwdLayout = new QGridLayout();
    frwdLayout->addWidget(modeLabel, 0, 0);
    frwdLayout->addWidget(mode, 0, 1);
    frwdLayout->addWidget(devWdg, 1, 0, 2, 2);
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

/* public slots */
QDomDocument Forward_Widget::getDataDocument() const
{
    _QWidget *wdg = static_cast<_QWidget*>(
                frwdModeSet->currentWidget());
    return wdg->getDataDocument();
}

/* private slots */
void Forward_Widget::modeChanged(const QString &_mode)
{
    bool state = ( _mode=="nat" || _mode=="route" );
    emit optionalsNeed(state);
    devWdg->setEnabled(state);
    if ( !state ) {
        devLabel->setChecked(false);
    };
}
