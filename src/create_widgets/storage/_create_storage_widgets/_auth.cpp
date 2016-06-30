#include "_auth.h"

_Storage_Auth::_Storage_Auth(
        QWidget *parent, virConnectPtr *connPtrPtr, QString _type) :
    _Changed(parent), ptr_ConnPtr(connPtrPtr), secType(_type)
{
    auth = new QCheckBox("Authentication", this);
    auth->setLayoutDirection(Qt::RightToLeft);
    userLabel = new QLabel("User Name:", this);
    userName = new QLineEdit(this);
    usageType = new QComboBox(this);
    usageType->addItems(QStringList()<<"Usage"<<"UUID");
    usage = new QLineEdit(this);
    usage->setPlaceholderText("libvirtiscsi");
    findSecret = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
    findSecret->setToolTip("Find/Create Secret");
    authLayout = new QGridLayout();
    authLayout->addWidget(userLabel, 0, 0);
    authLayout->addWidget(userName, 0, 1);
    authLayout->addWidget(usageType, 1, 0);
    authLayout->addWidget(usage, 1, 1);
    authLayout->addWidget(findSecret, 1, 2);
    authWdg = new QWidget(this);
    authWdg->setLayout(authLayout);
    authWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(auth, 0, Qt::AlignLeft);
    commonLayout->addWidget(authWdg, -1);
    setLayout(commonLayout);
    connect(auth, SIGNAL(toggled(bool)),
            authWdg, SLOT(setVisible(bool)));
    connect(auth, SIGNAL(stateChanged(int)),
            this, SLOT(changeAuthVisibility(int)));
    connect(usageType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(usageTypeChanged(int)));
    connect(auth, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(userName, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(usageType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(usage, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(findSecret, SIGNAL(clicked(bool)),
            this, SLOT(setSecret()));
}

/* public slots */
void _Storage_Auth::setSecretType(const QString &s)
{
    secType = s;
}
QString _Storage_Auth::getSecretType() const
{
    return secType;
}

/* private slots */
void _Storage_Auth::usageTypeChanged(int i)
{
    QString _type = usageType->currentText().toLower();
    if ( _type=="usage" ) {
        usage->setPlaceholderText("libvirtiscsi");
    } else if ( _type=="uuid" ) {
        usage->setPlaceholderText("3e3fce45-4f53-4fa7-bb32-11f34168b82b");
    };
    usage->clear();
}
void _Storage_Auth::setSecret()
{
    FindSecretDialog *findSecDialog =
            new FindSecretDialog(this, ptr_ConnPtr);
    int result = findSecDialog->exec();
    FSD_Result res = findSecDialog->getResult();
    findSecDialog->deleteLater();
    if ( (NOT_VOLUME!=secType.toUpper() && res.type.toLower()!=secType.toLower())
         || res.type.toLower()=="volume" ) {
        QString msg = QString("Type of secret should be is a %1")
                .arg(secType);
        findSecDialog->showMsg(msg);
        return;
    };
    if ( result==QDialog::Rejected ) return;
    QString _type = usageType->currentText().toLower();
    if ( _type=="usage" && !res.usage.isEmpty() ) {
        usage->setText(res.usage);
    } else if ( _type=="uuid" && !res.uuid.isEmpty() ) {
        usage->setText(res.uuid);
    }
}
void _Storage_Auth::changeAuthVisibility(int i)
{
    authWdg->setVisible( i==Qt::Checked );
}
