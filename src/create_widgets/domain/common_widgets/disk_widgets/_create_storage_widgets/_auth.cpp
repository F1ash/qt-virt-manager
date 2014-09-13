#include "_auth.h"

_Storage_Auth::_Storage_Auth(QWidget *parent) :
    QWidget(parent)
{
    auth = new QCheckBox("Authentication", this);
    auth->setLayoutDirection(Qt::RightToLeft);
    userLabel = new QLabel("User Name:", this);
    userName = new QLineEdit(this);
    usageType = new QComboBox(this);
    usageType->addItems(QStringList()<<"Usage"<<"UUID");
    usage = new QLineEdit(this);
    authLayout = new QGridLayout();
    authLayout->addWidget(userLabel, 0, 0);
    authLayout->addWidget(userName, 0, 1);
    authLayout->addWidget(usageType, 1, 0);
    authLayout->addWidget(usage, 1, 1);
    authWdg = new QWidget(this);
    authWdg->setLayout(authLayout);
    authWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(auth, 0, Qt::AlignLeft);
    commonLayout->addWidget(authWdg, -1, Qt::AlignRight);
    setLayout(commonLayout);
    connect(auth, SIGNAL(toggled(bool)),
            authWdg, SLOT(setEnabled(bool)));
}
