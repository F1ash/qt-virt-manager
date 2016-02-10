#include "spice_viewer.h"
#include <QApplication>
#include <QClipboard>

spcHlpThread::spcHlpThread(
        QObject *parent, virConnectPtr *connPtrPtr, QString _domain) :
    _VirtThread(parent, connPtrPtr), domain(_domain)
{

}
void spcHlpThread::run()
{
    if ( NULL==ptr_ConnPtr || NULL==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };
    domainPtr =virDomainLookupByName(
                    *ptr_ConnPtr, domain.toUtf8().data());
    domainIsActive = (virDomainIsActive(domainPtr)>0);
    uri.append(virConnectGetURI(*ptr_ConnPtr));
    // flag=0 for get running domain xml-description
    runXmlDesc.append( virDomainGetXMLDesc(domainPtr, 0) );
    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}

Spice_Viewer::Spice_Viewer(
        QWidget *parent, virConnectPtr *connPtrPtr,
        QString arg1, QString arg2) :
    VM_Viewer(parent, connPtrPtr, arg1, arg2)
{
    TYPE = "SPICE";
    hlpThread = new spcHlpThread(this, ptr_ConnPtr, domain);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init()));
    connect(hlpThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errorMsg(QString&)));
    hlpThread->start();
}

/* public slots */
void Spice_Viewer::init()
{
    // get address or hostname from URI
    // driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
    QString msg;
    if ( hlpThread->domainPtr!=NULL && hlpThread->domainIsActive ) {
        addr = hlpThread->uri.split("://").last();
        hlpThread->uri = addr;
        addr = hlpThread->uri.split("/").first();
        hlpThread->uri = addr;
        addr = hlpThread->uri.split(":").first();
        hlpThread->uri = addr;
        if ( hlpThread->uri.contains("@") ) {
            addr = hlpThread->uri.split("@").last();
            hlpThread->uri = addr;
        };
        addr.clear();
        QDomDocument doc;
        doc.setContent(hlpThread->runXmlDesc);
        QDomElement graph = doc.firstChildElement("domain")
           .firstChildElement("devices")
           .firstChildElement("graphics");
        //qDebug()<<doc.toByteArray(4);
        if (graph.hasAttribute("listen")) {
            // for listen address
            addr = graph.attribute("listen");
        } else if ( !graph.firstChildElement("listen").isNull() ) {
            // for listen address from virt.network
            addr = graph.firstChildElement("listen")
                    .attribute("address");
        } else {
            if ( hlpThread->uri.isEmpty() ) {
                addr = "127.0.0.1";
                hlpThread->uri  = "127.0.0.1";
            };
        };
        if ( addr!=hlpThread->uri && !hlpThread->uri.isEmpty() )
            addr = hlpThread->uri;
        port = (graph.hasAttribute("port"))?
                    graph.attribute("port").toInt() : 5900;
        //qDebug()<<"address:"<<addr<<port;
        if ( !graph.isNull() && graph.attribute("type")=="spice" ) {
            initSpiceWidget();
            actFullScreen = new QShortcut(
                        QKeySequence(tr("Shift+F11", "View|Full Screen")), this);
            connect(actFullScreen, SIGNAL(activated()),
                    SLOT(FullScreenTriggered()));
        } else {
            msg = QString("In '<b>%1</b>':<br> Unsupported type '%2'.<br> Use external Viewer.")
                    .arg(domain)
                    .arg((!graph.isNull())?
                             graph.attribute("type"):"???");
            sendErrMsg(msg);
            showErrorInfo(msg);
            startCloseProcess();
        };
    } else {
        viewerToolBar->setEnabled(false);
        msg = QString("In '<b>%1</b>':<br> Connection or Domain is NULL or inactive")
                .arg(domain);
        sendErrMsg(msg);
        showErrorInfo(msg);
        startCloseProcess();
    };
    sendConnErrors();
    //qDebug()<<msg<<"viewer inits";
}
void Spice_Viewer::reconnectToVirtDomain()
{
    QSpiceWidget *wdg = static_cast<QSpiceWidget*>(centralWidget());
    if ( NULL!=wdg ) {
        wdg->Disconnect();
        delete wdg;
        wdg = NULL;
        initSpiceWidget();
    };
}
void Spice_Viewer::sendKeySeqToVirtDomain(Qt::Key key)
{
    spiceWdg->SendKeySequience(key);
}
void Spice_Viewer::getScreenshotFromVirtDomain()
{
    spiceWdg->getScreenshot();
}
void Spice_Viewer::copyFilesToVirtDomain()
{
    if ( NULL==spiceWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, "Copy files to Guest", "~");
    spiceWdg->mainFileCopyAsync(fileNames);
}
void Spice_Viewer::copyToClipboardFromVirtDomain()
{
    if ( NULL==spiceWdg ) return;
    spiceWdg->copyClipboardFromGuest();
}
void Spice_Viewer::pasteClipboardToVirtDomain()
{
    if ( NULL==spiceWdg ) return;
    QString _text = QApplication::clipboard()->text(QClipboard::Clipboard);
    QImage _image = QApplication::clipboard()->image(QClipboard::Clipboard);
    //qDebug()<<"copy:"<<_text<<_image.text()<<";";
    if ( !_text.isEmpty() ) {
        spiceWdg->sendClipboardDataToGuest(
                    VD_AGENT_CLIPBOARD_UTF8_TEXT,
                    (const uchar*)_text.toUtf8().data(),
                    _text.size());
    };
    if ( !_image.isNull() ) {
        QString _format = _text.split(".").last();
        qint32 _frmt;
        if ( _format.isEmpty() ) {
            QMessageBox::information(
                        this,
                        "INFO",
                        QString("Unknown image format:\n'%1'")
                        .arg(_text));
            return;
        } else if ( _format.toLower()=="png" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_PNG;
        } else if ( _format.toLower()=="bmp" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_BMP;
        } else if ( _format.toLower()=="jpg" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_JPG;
        } else if ( _format.toLower()=="tiff" ) {
            _frmt = VD_AGENT_CLIPBOARD_IMAGE_TIFF;
        } else {
            QMessageBox::information(
                        this,
                        "INFO",
                        "Unknown image format.");
            return;
        };
        spiceWdg->sendClipboardDataToGuest(
                    _frmt,
                    _image.constBits(),
                    _image.byteCount());
    };
}

/* private slots */
void Spice_Viewer::initSpiceWidget()
{
    spiceWdg = new QSpiceWidget(this);
    setCentralWidget(spiceWdg);
    QString _name = QString("%1[%2]").arg(domain).arg(connName);
    spiceWdg->setGuestName(_name);
    connect(spiceWdg, SIGNAL(DisplayResize(const QSize&)),
            SLOT(DisplayResize(const QSize&)));
    connect(spiceWdg, SIGNAL(downloaded(int,int)),
            vm_stateWdg, SLOT(setDownloadProcessValue(int,int)));
    connect(spiceWdg, SIGNAL(displayChannelChanged(bool)),
            vm_stateWdg, SLOT(changeDisplayState(bool)));
    connect(spiceWdg, SIGNAL(cursorChannelChanged(bool)),
            vm_stateWdg, SLOT(changeMouseState(bool)));
    connect(spiceWdg, SIGNAL(inputsChannelChanged(bool)),
            vm_stateWdg, SLOT(changeKeyboardState(bool)));
    connect(spiceWdg, SIGNAL(usbredirChannelChanged(bool)),
            vm_stateWdg, SLOT(changeUsbredirState(bool)));
    connect(spiceWdg, SIGNAL(smartcardChannelChanged(bool)),
            vm_stateWdg, SLOT(changeSmartcardState(bool)));
    connect(spiceWdg, SIGNAL(webdavChannelChanged(bool)),
            vm_stateWdg, SLOT(changeWebDAVState(bool)));
    connect(spiceWdg, SIGNAL(playbackChannelChanged(bool)),
            vm_stateWdg, SLOT(changePlaybackState(bool)));
    connect(spiceWdg, SIGNAL(recordChannelChanged(bool)),
            vm_stateWdg, SLOT(changeRecordState(bool)));
    connect(vm_stateWdg, SIGNAL(showUsbDevWidget()),
            spiceWdg, SLOT(showUsbDevWidget()));
    connect(vm_stateWdg, SIGNAL(showSmartCardWidget()),
            spiceWdg, SLOT(showSmartCardWidget()));
    connect(vm_stateWdg, SIGNAL(transformationMode(Qt::TransformationMode)),
            spiceWdg, SLOT(setTransformationMode(Qt::TransformationMode)));
    connect(spiceWdg, SIGNAL(errMsg(QString&)),
            this, SLOT(sendErrMsg(QString&)));

    QSize around_size = getWidgetSizeAroundDisplay();
    QString _uri = QString("spice://%1:%2").arg(addr).arg(port);
    spiceWdg->setNewSize(around_size.width(), around_size.height());
    spiceWdg->Connect(_uri);
}

void Spice_Viewer::timerEvent(QTimerEvent *ev)
{
    if ( ev->timerId()==killTimerId ) {
        counter++;
        vm_stateWdg->setCloseProcessValue(counter*PERIOD*6);
        if ( TIMEOUT<counter*PERIOD*6 ) {
            killTimer(killTimerId);
            killTimerId = 0;
            counter = 0;
            close();
        };
    }
}

void Spice_Viewer::DisplayResize(const QSize &size)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    resize(size+around_size);
}

void Spice_Viewer::FullScreenTriggered()
{
    if (isFullScreen())
        setWindowState(Qt::WindowNoState);
    else
        setWindowState(Qt::WindowFullScreen);
}

void Spice_Viewer::resizeEvent(QResizeEvent *ev)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    if ( NULL!=spiceWdg ) {
        spiceWdg->setNewSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    };
}

QSize Spice_Viewer::getWidgetSizeAroundDisplay()
{
    int left, top, right, bottom, _width, _height;
    viewerToolBar->getContentsMargins(&left, &top, &right, &bottom);
    _width = left+right;
    _height = top +bottom;
    vm_stateWdg->getContentsMargins(&left, &top, &right, &bottom);
    _width += left+right;
    _height += top +bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    _width += left+right;
    _height += vm_stateWdg->size().height()
            +viewerToolBar->size().height()
            +top +bottom;
    QSize _size(_width, _height);
    return _size;
}
