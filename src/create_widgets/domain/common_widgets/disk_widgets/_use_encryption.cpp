#include "_use_encryption.h"

_UseEncryption::_UseEncryption(QWidget *parent, virConnectPtr _conn) :
    QWidget(parent), currWorkConnection(_conn)
{
    usage = new QCheckBox("Use Encryption", this);
    usage->setLayoutDirection(Qt::RightToLeft);
    secUsage = new QLineEdit(this);
    findSecret = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
    findSecret->setToolTip("Find Secret");
    baseLayout = new QHBoxLayout(this);
    baseLayout->addWidget(secUsage);
    baseLayout->addWidget(findSecret);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    info = new QLabel(this);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(info);
    commonLayout->setAlignment(Qt::AlignLeft);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(findSecret, SIGNAL(released()),
            this, SLOT(setVolumeSecret()));
    // dataChanged signals
    connect(usage, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(secUsage, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
bool _UseEncryption::isUsed() const
{
    return usage->isChecked();
}
void _UseEncryption::setUsage(bool state)
{
    usage->setChecked(state);
}
QString _UseEncryption::getSecretUUID() const
{
    return secUsage->text();
}
void _UseEncryption::setSecretUUID(const QString &s)
{
    secUsage->setText(s);
}

/* private slots */
void _UseEncryption::setVolumeSecret()
{
    FindSecretDialog *findSecDialog =
            new FindSecretDialog(this, currWorkConnection);
    int result = findSecDialog->exec();
    FSD_Result res = findSecDialog->getResult();
    findSecDialog->deleteLater();
    if ( "VOLUME"!=res.type.toUpper() ) {
        QString msg = QString("Type of secret should be is a VOLUME");
        findSecDialog->showMsg(msg);
        return;
    };
    if ( result==QDialog::Rejected ) return;
    secUsage->setText(res.uuid);
}
