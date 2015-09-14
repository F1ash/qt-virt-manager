#include "_use_encryption.h"

_UseEncryption::_UseEncryption(QWidget *parent, virConnectPtr *connPtr) :
    QWidget(parent), ptr_ConnPtr(connPtr)
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
    connect(secUsage, SIGNAL(textChanged(QString)),
            this, SLOT(emitSecretList()));
    // dataChanged signals
    connect(usage, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(secUsage, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));

    thread = new SecretControlThread(this);
    connect(thread, SIGNAL(resultData(Result)),
            this, SLOT(resultReceiver(Result)));
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
void _UseEncryption::emitSecretList()
{
    TASK _task;
    _task.type          = "secret";
    _task.srcConnPtr    = ptr_ConnPtr;
    _task.action        = GET_ALL_ENTITY_STATE;
    thread->execAction(0, _task);
}
void _UseEncryption::setCurrVolumePath(const QString &s)
{
    currVolumePath = s;
    emitSecretList();
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
void _UseEncryption::resultReceiver(Result data)
{
    //qDebug()<<currVolumePath<<"currVolumePath";
    /*
     * after set secUsage->setText() then emitted textChanged()
     * and find secret for current volume path
     */
    QString _text;
    QStringList _secUUIDs, _secUsages;
    foreach (QString _sec, data.msg) {
        QStringList _data = _sec.split(DFR);
        _secUUIDs.append(_data.at(0));
        _secUsages.append(_data.at(1));
    };
    if ( currVolumePath.isEmpty() ) {
        if ( _secUUIDs.contains(secUsage->text()) ) {
            _text = "Secret is exist";
        } else {
            _text = "Secret isn't exist";
        }
    } else {
        if ( _secUsages.contains(currVolumePath) ) {
            _text = "Secret is exist for current volume";
            int i = _secUsages.indexOf(currVolumePath);
            secUsage->setText(_secUUIDs.at(i));
        } else {
            _text = "Secret isn't exist for current volume";
            secUsage->clear();
        };
    };
    info->setText(_text);
}
void _UseEncryption::setVolumeSecret()
{
    FindSecretDialog *findSecDialog =
            new FindSecretDialog(this, ptr_ConnPtr);
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
