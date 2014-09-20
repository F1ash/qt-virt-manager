#include "_create_storage.h"

_CreateStorage::_CreateStorage(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
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

    chooseStorage = new QPushButton(QIcon::fromTheme("dialog-ok"), "Choose Storage", this);
    cancel = new QPushButton(QIcon::fromTheme("dialog-cancel"), "Cancel", this);
    connect(chooseStorage, SIGNAL(clicked()), this, SLOT(set_Result()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(set_Result()));
    buttonLayout = new QHBoxLayout(this);
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
    xml->setFileTemplate(QString("%1%2XML_Desc-XXXXXX.xml")
                         .arg(QDir::tempPath())
                         .arg(QDir::separator()));

    connect(type, SIGNAL(currentIndexChanged(int)),
            info, SLOT(setCurrentIndex(int)));
}

/* public slots */
QString _CreateStorage::getStorageXMLDescFileName() const
{
    return QString();
}

/* private slots */
void _CreateStorage::set_Result()
{
    done( (sender()==chooseStorage)?
              QDialog::Accepted :
              QDialog::Rejected);
    //qDebug()<<"done";
}
