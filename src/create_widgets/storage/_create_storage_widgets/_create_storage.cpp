#include "_create_storage.h"

_CreateStorage::_CreateStorage(QWidget *parent) :
    QDialog(parent)
{
    setModal(false);
    typeLabel = new QLabel("Pool Type:", this);
    type = new QComboBox(this);
    stNameLabel = new QLabel("Name:", this);
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
                "Show XML Description\nat close", this);
    chooseStorage = new QPushButton(
                QIcon::fromTheme("dialog-ok"),
                "Choose Storage",
                this);
    cancel = new QPushButton(
                QIcon::fromTheme("dialog-cancel"),
                "Cancel",
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
                QString("<a href='%1'>About</a>")
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
