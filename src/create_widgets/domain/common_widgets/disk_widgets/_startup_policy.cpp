#include "_startup_policy.h"

_StartupPolicy::_StartupPolicy(QWidget *parent) :
    QWidget(parent)
{
    startupPolicyLabel = new QCheckBox("startupPolicy:", this);
    startupPolicyLabel->setLayoutDirection(Qt::RightToLeft);
    startupPolicy = new QComboBox(this);
    startupPolicy->addItems(QStringList()<<"mandatory"<<"requisite"<<"optional");
    startupPolicy->setEnabled(false);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(startupPolicyLabel);
    commonLayout->addWidget(startupPolicy);
    setLayout(commonLayout);
    connect(startupPolicyLabel, SIGNAL(toggled(bool)),
            startupPolicy, SLOT(setEnabled(bool)));
}

/* public slots */
bool _StartupPolicy::isUsed() const
{
    return startupPolicyLabel->isChecked();
}
QString _StartupPolicy::getStartupPolicy() const
{
    return startupPolicy->currentText();
}
