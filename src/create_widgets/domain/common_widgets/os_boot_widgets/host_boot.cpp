#include "host_boot.h"

Host_Boot::Host_Boot(QWidget *parent) :
    _QWidget(parent)
{
    loaderLabel = new QLabel("Boot loader:", this);
    argsLabel = new QLabel("Boot loader args:", this);
    loader = new Path_To_File(this);
    QString _placeHolderText = QString("/usr/bin/pygrub");
    loader->setPlaceholderText(_placeHolderText);
    args = new QLineEdit(this);
    args->setPlaceholderText("--append single");
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(loaderLabel);
    commonLayout->addWidget(loader);
    commonLayout->addWidget(argsLabel);
    commonLayout->addWidget(args);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(loader, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(args, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Host_Boot::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _bootloader, _bootloader_args, _data;
    _data = doc.createElement("data");
    _bootloader = doc.createElement("bootloader");
    _bootloader_args = doc.createElement("bootloader_args");
    _data.appendChild(_bootloader);
    _data.appendChild(_bootloader_args);

    data = doc.createTextNode(loader->getPath());
    _bootloader.appendChild(data);
    data = doc.createTextNode(args->text());
    _bootloader_args.appendChild(data);
    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void Host_Boot::setDataDescription(QString &_xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _bootloader, _bootloader_args;
    _bootloader = doc
            .firstChildElement("domain")
            .firstChildElement("bootloader");
    _bootloader_args = doc
            .firstChildElement("domain")
            .firstChildElement("bootloader_args");
    QString _attr;
    if ( !_bootloader.isNull() ) {
        _attr = _bootloader
                .firstChild().toText().data();
        loader->setPath(_attr);
    } else
        loader->clear();
    if ( !_bootloader_args.isNull() ) {
        _attr = _bootloader_args
                .firstChild().toText().data();
        args->setText(_attr);
    } else
        args->clear();
}
