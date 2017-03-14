#include "_use_encryption.h"

_UseEncryption::_UseEncryption(QWidget *parent, virConnectPtr *connPtrPtr) :
    _Changed(parent), ptr_ConnPtr(connPtrPtr)
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
    commonLayout->addWidget(info);
    commonLayout->addWidget(baseWdg);
    commonLayout->setAlignment(Qt::AlignLeft);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(findSecret, SIGNAL(released()),
            this, SLOT(setVolumeSecret()));
    // dataChanged signals
    connect(usage, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(secUsage, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));

    hlpThread = new SecretControlThread(this);
    connect(hlpThread, SIGNAL(resultData(Result)),
            this, SLOT(resultReceiver(Result)));
    connect(hlpThread, SIGNAL(errorMsg(const QString&, const uint)),
            this, SIGNAL(errorMsg(const QString&)));
    connect(secUsage, SIGNAL(textChanged(QString)),
            this, SLOT(searchInSecretList()));
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(emitCompleteSignal()));
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
void _UseEncryption::setCurrVolumePath(const QString &s)
{
    // is only used when changing the current volume path
    currVolumePath = s;
    searchInSecretList();
}
QString _UseEncryption::getSecretUUID() const
{
    return secUsage->text();
}
void _UseEncryption::setSecretUUID(const QString &s)
{
    // used once at setting xmlData Description from ****_disk
    secUsage->setText(s);
}

/* private slots */
void _UseEncryption::searchInSecretList()
{
    // start only if thread not running, because this thread
    // can be used from different place spontaneously
    if ( hlpThread->isRunning() ) return;
    TASK _task;
    _task.type          = VIRT_ENTITY::VIRT_SECRET;
    _task.srcConnPtr    = ptr_ConnPtr;
    _task.action        = Actions::GET_ALL_ENTITY_STATE;
    hlpThread->execAction(0, _task);
}
void _UseEncryption::resultReceiver(Result data)
{
    //qDebug()<<currVolumePath<<"currVolumePath";
    /*
     * after set secUsage->setText() will emitted textChanged()
     * and search the secret for current volume path will started
     */
    QString _text;
    QStringList _secUUIDs, _secUsages;
    foreach (QVariantMap _sec, data.data) {
        _secUUIDs.append(_sec.value("UUID").toString());
        _secUsages.append(_sec.value("UsageID").toString());
    };
    // block the signals for dropping indefinite loop,
    // because textChanged() signal emitted,
    // when text set programmatically
    secUsage->blockSignals(true);
    bool secExist = false;
    if ( currVolumePath.isEmpty() ) {
        if ( _secUUIDs.contains(secUsage->text()) ) {
            _text = "Secret is exist";
            secExist = true;
        } else {
            _text = "Secret isn't exist";
        }
    } else {
        if ( _secUsages.contains(currVolumePath) ) {
            _text = "Secret is exist for current volume";
            int i = _secUsages.indexOf(currVolumePath);
            secUsage->setText(_secUUIDs.at(i));
            secExist = true;
        } else {
            _text = "Secret isn't exist for current volume";
            secUsage->clear();
        };
    };
    secUsage->setStyleSheet(
                QString("QLineEdit {background-color : %1;}")
                .arg((secExist)? "green" : "red"));
    // unblock signals
    secUsage->blockSignals(false);
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
void _UseEncryption::emitCompleteSignal()
{
    if ( sender()==hlpThread ) {
        setEnabled(true);
        emit complete();
    }
}
