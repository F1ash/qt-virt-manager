#include "events.h"

#define COMMON_ACTIONS QStringList()\
    <<"destroy"\
    <<"restart"\
    <<"preserve"\
    <<"rename-restart"
#define ADDITIONAL_ACTIONS QStringList()\
    <<"coredump-destroy"\
    <<"coredump-restart"
#define LOCK_FAILURE_ACTIONS QStringList()\
    <<"poweroff"\
    <<"restart"\
    <<"pause"\
    <<"ignore"

Events::Events(QWidget *parent) :
    QWidget(parent)
{
    powerOffLabel = new QCheckBox("PowerOff", this);
    rebootLabel = new QCheckBox("Reboot", this);
    crashLabel = new QCheckBox("Crash", this);
    lockFailureLabel = new QCheckBox("LockFailure", this);
    on_powerOff = new QComboBox(this);
    on_powerOff->addItems(COMMON_ACTIONS);
    on_reboot = new QComboBox(this);
    on_reboot->addItems(COMMON_ACTIONS);
    on_crash = new QComboBox(this);
    on_crash->addItems(COMMON_ACTIONS);
    on_crash->addItems(ADDITIONAL_ACTIONS);
    on_lockfailure = new QComboBox(this);
    on_lockfailure->addItems(LOCK_FAILURE_ACTIONS);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(powerOffLabel, 0, 0);
    commonLayout->addWidget(on_powerOff, 1, 0);
    commonLayout->addWidget(rebootLabel, 0, 1);
    commonLayout->addWidget(on_reboot, 1, 1);
    commonLayout->addWidget(crashLabel, 0, 2);
    commonLayout->addWidget(on_crash, 1, 2);
    commonLayout->addWidget(lockFailureLabel, 0, 3);
    commonLayout->addWidget(on_lockfailure, 1, 3);
    setLayout(commonLayout);
    connect(powerOffLabel, SIGNAL(toggled(bool)),
            on_powerOff, SLOT(setEnabled(bool)));
    connect(rebootLabel, SIGNAL(toggled(bool)),
            on_reboot, SLOT(setEnabled(bool)));
    connect(crashLabel, SIGNAL(toggled(bool)),
            on_crash, SLOT(setEnabled(bool)));
    connect(lockFailureLabel, SIGNAL(toggled(bool)),
            on_lockfailure, SLOT(setEnabled(bool)));
    powerOffLabel->setChecked(false);
    rebootLabel->setChecked(false);
    crashLabel->setChecked(false);
    lockFailureLabel->setChecked(false);
    on_powerOff->setEnabled(false);
    on_reboot->setEnabled(false);
    on_crash->setEnabled(false);
    on_lockfailure->setEnabled(false);
    // dataChanged connections
    connect(powerOffLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(rebootLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(crashLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(lockFailureLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(on_powerOff, SIGNAL(currentIndexChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(on_reboot, SIGNAL(currentIndexChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(on_crash, SIGNAL(currentIndexChanged(QString)),
            this, SIGNAL(dataChanged()));
    connect(on_lockfailure, SIGNAL(currentIndexChanged(QString)),
            this, SIGNAL(dataChanged()));
}
