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
    usage->setPlaceholderText("libvirtiscsi");
    authLayout = new QGridLayout();
    authLayout->addWidget(userLabel, 0, 0);
    authLayout->addWidget(userName, 0, 1);
    authLayout->addWidget(usageType, 1, 0);
    authLayout->addWidget(usage, 1, 1);
    authWdg = new QWidget(this);
    authWdg->setLayout(authLayout);
    authWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(auth, 0, Qt::AlignLeft);
    commonLayout->addWidget(authWdg, -1);
    setLayout(commonLayout);
    connect(auth, SIGNAL(toggled(bool)),
            authWdg, SLOT(setVisible(bool)));
    connect(usageType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(usageTypeChanged(int)));
    connect(auth, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(userName, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
    connect(usageType, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(usage, SIGNAL(textEdited(QString)),
            this, SIGNAL(dataChanged()));
}

/* private slots */
void _Storage_Auth::usageTypeChanged(int i)
{
    QString _type = usageType->currentText().toLower();
    if ( _type=="usage" ) {
        usage->setPlaceholderText("libvirtiscsi");
    } else if ( _type=="uuid" ) {
        usage->setPlaceholderText("3e3fce45-4f53-4fa7-bb32-11f34168b82b");
    }
}
