#include "encryption.h"
#include "create_widgets/storage/_create_storage_widgets/find_secret_dialog.h"

Encryption::Encryption(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr) :
    QWidget(parent), ptr_ConnPtr(connPtrPtr)
{
    setAutoFillBackground(true);
    useEncryption = new QCheckBox(
                tr("Use Encryption"), this);
    formatLabel = new QLabel(tr("Format:"), this);
    format = new QComboBox(this);
    format->addItems(QStringList()<<"default"<<"qcow"<<"luks");
    autoSecret = new QCheckBox(tr("Auto-generate Secret"), this);
    autoSecret->setChecked(true);
    secUUID = new QLabel(this);
    findSecret = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
    findSecret->setToolTip(tr("Find Secret"));
    secretLayout = new QHBoxLayout(this);
    secretWdg = new QWidget(this);
    secretWdg->setLayout(secretLayout);
    secretLayout->addWidget(secUUID, 0, Qt::AlignLeft);
    secretLayout->addWidget(findSecret, 0, Qt::AlignRight);
    secretWdg->setEnabled(false);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(formatLabel, 0, 0);
    baseLayout->addWidget(format, 0, 1);
    baseLayout->addWidget(autoSecret, 1, 0);
    baseLayout->addWidget(secretWdg, 2, 0, 3, 2);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useEncryption);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);

    connect(useEncryption, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(autoSecret, SIGNAL(toggled(bool)),
            secretWdg, SLOT(setDisabled(bool)));
    connect(findSecret, SIGNAL(released()),
            this, SLOT(setSecret()));
}

bool Encryption::isUsed() const
{
    return useEncryption->isChecked();
}
void Encryption::setUsage(const bool state)
{
    useEncryption->setChecked(state);
}
QString Encryption::getFormat() const
{
    return format->currentText().toLower();
}
void Encryption::setFormat(const QString &_fmt)
{
    int idx = format->findText(_fmt);
    if ( idx<0 ) idx = 0;
    format->setCurrentIndex(idx);
}
bool Encryption::AutoSecretIsUsed() const
{
    return autoSecret->isChecked();
}
void Encryption::setAutoSecretUsage(const bool state)
{
    autoSecret->setChecked(state);
}
QString Encryption::getSecretUUID() const
{
    if ( !autoSecret->isChecked() ) {
        return secUUID->text();
    };
    return QString();
}
void Encryption::setSecretUUID(const QString &_secUUID)
{
    secUUID->setText(_secUUID);
}

/* private slots */
void Encryption::setSecret()
{
    FindSecretDialog *findSecDialog =
            new FindSecretDialog(this, ptr_ConnPtr);
    int result = findSecDialog->exec();
    FSD_Result res = findSecDialog->getResult();
    findSecDialog->deleteLater();
    if ( res.type.toUpper().compare("VOLUME")!=0 ) {
        QString msg = tr("Type of secret should be is a VOLUME");
        findSecDialog->showMsg(msg);
        return;
    };
    if ( result==QDialog::Rejected ) return;
    secUUID->setText(res.uuid);
}
