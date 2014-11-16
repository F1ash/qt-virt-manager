#include "direct_kernel_boot.h"

Direct_Kernel_Boot::Direct_Kernel_Boot(QWidget *parent) :
    _QWidget(parent)
{
    loaderLabel = new QLabel("Boot loader path:", this);
    kernelLabel = new QLabel("Kernel path:", this);
    initrdLabel = new QLabel("Initrd path:", this);
    cmdlineLabel = new QLabel("Command line:", this);
    dtbLabel = new QLabel("Path to the (optional) device tree binary:", this);
    loader = new Path_To_File(this);
    QString _placeHolderText = QString("/usr/lib/xen/boot/hvmloader");
    loader->setPlaceholderText(_placeHolderText);
    kernel = new Path_To_File(this);
    _placeHolderText = QString("/root/f21-i386-vmlinuz");
    kernel->setPlaceholderText(_placeHolderText);
    initrd = new Path_To_File(this);
    _placeHolderText = QString("/root/f21-i386-initrd");
    initrd->setPlaceholderText(_placeHolderText);
    cmdline = new QLineEdit(this);
    cmdline->setPlaceholderText("console=ttyS0 ks=http://example.com/f21-i386/os/");
    dtb = new Path_To_File(this);
    _placeHolderText = QString("/root/ppc.dtb");
    dtb->setPlaceholderText(_placeHolderText);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(loaderLabel);
    commonLayout->addWidget(loader);
    commonLayout->addWidget(kernelLabel);
    commonLayout->addWidget(kernel);
    commonLayout->addWidget(initrdLabel);
    commonLayout->addWidget(initrd);
    commonLayout->addWidget(cmdlineLabel);
    commonLayout->addWidget(cmdline);
    commonLayout->addWidget(dtbLabel);
    commonLayout->addWidget(dtb);
    commonLayout->insertStretch(-1);
    setLayout(commonLayout);
    // dataChanged connections
    connect(loader, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(kernel, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(initrd, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
    connect(cmdline, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(dtb, SIGNAL(dataChanged()),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Direct_Kernel_Boot::getDataDocument() const
{
    QDomText data;
    QDomDocument doc;
    QDomElement _data, _os, _loader, _type,
              _kernel, _initrd, _cmdline, _dtb;
    _data = doc.createElement("data");
    _os = doc.createElement("os");
    _loader = doc.createElement("loader");
    _type = doc.createElement("type");
    _kernel = doc.createElement("kernel");
    _initrd = doc.createElement("initrd");
    _cmdline = doc.createElement("cmdline");
    _dtb = doc.createElement("dtb");
    _data.appendChild(_os);
    _os.appendChild(_type);
    _os.appendChild(_loader);
    _os.appendChild(_kernel);
    _os.appendChild(_initrd);
    _os.appendChild(_cmdline);
    _os.appendChild(_dtb);

    data = doc.createTextNode(loader->getPath());
    _loader.appendChild(data);
    data = doc.createTextNode(kernel->getPath());
    _kernel.appendChild(data);
    data = doc.createTextNode(initrd->getPath());
    _initrd.appendChild(data);
    data = doc.createTextNode(cmdline->text());
    _cmdline.appendChild(data);
    data = doc.createTextNode(dtb->getPath());
    _dtb.appendChild(data);

    doc.appendChild(_data);
    //qDebug()<<doc.toString();
    return doc;
}
void Direct_Kernel_Boot::setDataDescription(QString &_xmlDesc)
{
    //qDebug()<<xmlDesc;
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _os, _loader, _type,
              _kernel, _initrd, _cmdline, _dtb;
    _os = doc
            .firstChildElement("domain")
            .firstChildElement("os");
    _loader = _os.firstChildElement("loader");
    _type = _os.firstChildElement("type");
    _kernel = _os.firstChildElement("kernel");
    _initrd = _os.firstChildElement("initrd");
    _cmdline = _os.firstChildElement("cmdline");
    _dtb = _os.firstChildElement("dtb");
    QString _attr;
    if ( !_loader.isNull() ) {
        _attr = _loader
                .firstChild().toText().data();
        loader->setPath(_attr);
    } else
        loader->clear();
    if ( !_kernel.isNull() ) {
        _attr = _kernel
                .firstChild().toText().data();
        kernel->setPath(_attr);
    } else
        kernel->clear();
    if ( !_initrd.isNull() ) {
        _attr = _initrd
                .firstChild().toText().data();
        initrd->setPath(_attr);
    } else
        initrd->clear();
    if ( !_cmdline.isNull() ) {
        cmdline->setText(
                    _cmdline
                    .firstChild().toText().data());
    } else
        cmdline->clear();
    if ( !_dtb.isNull() ) {
        _attr = _dtb
                .firstChild().toText().data();
        dtb->setPath(_attr);
    } else
        dtb->clear();
}
