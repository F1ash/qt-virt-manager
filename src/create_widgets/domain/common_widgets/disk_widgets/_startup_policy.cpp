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
    connect(startupPolicyLabel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(startupPolicy, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
bool _StartupPolicy::isUsed() const
{
    return startupPolicyLabel->isChecked();
}
void _StartupPolicy::setUsage(bool state)
{
    startupPolicyLabel->setChecked(state);
}
QString _StartupPolicy::getStartupPolicy() const
{
    return startupPolicy->currentText();
}
int _StartupPolicy::findPolicyIndex(QString &_startupPolicy)
{
    int idx = startupPolicy->findText(
                _startupPolicy,
                Qt::MatchContains);
    return idx;
}
void _StartupPolicy::setPolicyIndex(int idx)
{
    startupPolicy->setCurrentIndex(idx);
}
