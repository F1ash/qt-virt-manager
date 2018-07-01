#include "_create_storage.h"

_CreateStorage::_CreateStorage(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _xmlFile) :
    QDialog(parent), ptr_ConnPtr(connPtrPtr), xmlFileName(_xmlFile)
{
    setModal(false);
    typeLabel = new QLabel(tr("Pool Type:"), this);
    type = new QComboBox(this);
    stNameLabel = new QLabel(tr("Name:"), this);
    stName = new QLineEdit(this);
    suff = new QLabel(".img", this);
    suff->setVisible(false);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(typeLabel, 0, 0);
    baseLayout->addWidget(type, 0, 1);
    baseLayout->addWidget(stNameLabel, 1, 0);
    baseLayout->addWidget(stName, 1, 1);
    baseLayout->addWidget(suff, 1, 2);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);

    about = new QLabel(this);
    about->setOpenExternalLinks(true);
    showAtClose = new QCheckBox(
                tr("Show XML Description\nat close"), this);
    chooseStorage = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                tr("Choose Storage"),
                this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"),
                tr("Cancel"),
                this);
    connect(chooseStorage, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(about);
    buttonLayout->addWidget(showAtClose);
    buttonLayout->addWidget(chooseStorage);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);

    info = new QStackedWidget(this);
    infoLayout = new QVBoxLayout();
    infoLayout->addWidget(info);
    scrolled = new QWidget(this);
    scrolled->setLayout(infoLayout);
    infoWidget = new QScrollArea(this);
    infoWidget->setWidget(scrolled);
    infoWidget->setWidgetResizable(true);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(baseWdg);
    commonLayout->addWidget(buttons);
    setLayout(commonLayout);

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));

    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}
void _CreateStorage::readXMLDataDescription()
{
    if ( xmlFileName.isEmpty() ) {
        // create/define new Storage entity
    } else {
        // read for edit exist Storage entity
        QString xmlDesc;
        QFile *_xml = new QFile(this);
        _xml->setFileName(xmlFileName);
        bool opened = _xml->open(QIODevice::ReadOnly);
        if ( opened ) xmlDesc.append(_xml->readAll().constData());
        _xml->close();
        _xml->deleteLater();
        if ( opened ) setDataDescription(xmlDesc);
    };
}
void _CreateStorage::setDataDescription(const QString &_xmlDesc)
{
    Q_UNUSED(_xmlDesc);
}
quint64 _CreateStorage::convertNiBtoMiB(quint64 _NiB, const QString &_unit)
{
    if ( _unit.compare("b")==0 || _unit.compare("bytes")==0 ) {
        quint64 _res = _NiB / 1048576;
        return (_res==0)? 1 : _res;
    } else if ( _unit.compare("K")==0 || _unit.compare("KiB")==0 ) {
        return _NiB / 1024;
    } else if ( _unit.compare("KB")==0 ) {
        return convertNiBtoMiB(_NiB*1000, "b");
    } else if ( _unit.compare("M")==0 || _unit.compare("MiB")==0 ) {
        return _NiB;
    } else if ( _unit.compare("MB")==0 ) {
        return _NiB*1000 / 1024;
    } else if ( _unit.compare("G")==0 || _unit.compare("GiB")==0 ) {
        return _NiB*1024;
    } else if ( _unit.compare("GB")==0 ) {
        return _NiB*1000000/1024;
    } else if ( _unit.compare("T")==0 || _unit.compare("TiB")==0 ) {
        return _NiB*1048576;
    } else if ( _unit.compare("TB")==0 ) {
        return _NiB*1000000000/1024;
    } else if ( _unit.compare("E")==0 || _unit.compare("EiB")==0 ) {
        return _NiB*1073741824;
    } else if ( _unit.compare("EB")==0 ) {
        return _NiB*1000000000000/1024;
    } else return 0;
}

/* public slots */
QString _CreateStorage::getXMLDescFileName() const
{
    return QString();
}
bool _CreateStorage::showXMLDescription() const
{
    return showAtClose->isChecked();
}
void _CreateStorage::setUrl(QString _url)
{
    about->setText(
                QString(tr("<a href='%1'>About</a>"))
                .arg(_url));
    about->setToolTip(_url);
}

/* private slots */
void _CreateStorage::set_Result()
{
    setResult( (sender()==chooseStorage)?
                QDialog::Accepted :
                QDialog::Rejected );
    done(result());
    //qDebug()<<"done";
    if ( !settingName.isEmpty() ) {
        settings.beginGroup(settingName);
        settings.setValue(
                    "Geometry",
                    saveGeometry());
        settings.setValue(
                    "ShowAtClose",
                    showAtClose->isChecked());
        settings.endGroup();
    };
}
