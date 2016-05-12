#include "vnc_viewer.h"
#include <QApplication>
#include <QClipboard>
#include <spice/vd_agent.h>

vncHlpThread::vncHlpThread(
        QObject *parent, virConnectPtr *connPtrPtr, QString _domain) :
    _VirtThread(parent, connPtrPtr), domain(_domain)
{

}
void vncHlpThread::run()
{
    if ( nullptr==ptr_ConnPtr || nullptr==*ptr_ConnPtr ) {
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

VNC_Viewer::VNC_Viewer(
        QWidget *parent, virConnectPtr *connPtrPtr,
        QString arg1, QString arg2) :
    VM_Viewer(parent, connPtrPtr, arg1, arg2)
{
    TYPE = "VNC";
    hlpThread = new vncHlpThread(this, ptr_ConnPtr, domain);
    connect(hlpThread, SIGNAL(finished()),
            this, SLOT(init()));
    connect(hlpThread, SIGNAL(errorMsg(QString&,uint)),
            this, SIGNAL(errorMsg(QString&)));
    hlpThread->start();
}

/* public slots */
void VNC_Viewer::init()
{
    guestName = QString("%1[%2]").arg(domain).arg(connName);
    // get address or hostname from URI
    // driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
    QString msg;
    if ( hlpThread->domainPtr!=nullptr && hlpThread->domainIsActive ) {
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
        if ( !graph.isNull() && graph.attribute("type")=="vnc" ) {
            initVNCWidget();
            actFullScreen = new QShortcut(
                        QKeySequence(tr("Shift+F11", "View|Full Screen")), this);
            connect(actFullScreen, SIGNAL(activated()),
                    SLOT(fullScreenTriggered()));
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
void VNC_Viewer::reconnectToVirtDomain()
{
    if ( nullptr!=vncWdg ) {
        //delete vncWdg;
        //vncWdg = nullptr;
        // resizing to any,
        // because will need to init new display configuration
        //resize(getWidgetSizeAroundDisplay());
        vncWdg->reinitVNC();
        QSize around_size = getWidgetSizeAroundDisplay();
        resize(around_size);
        if ( nullptr!=vncWdg ) {
            vncWdg->newViewSize(
                        size().width()-around_size.width(),
                        size().height()-around_size.height());
        };
    };
}
void VNC_Viewer::sendKeySeqToVirtDomain(Qt::Key key)
{
    if ( nullptr==vncWdg ) return;
    //vncWdg->sendKeySequience(key);
}
void VNC_Viewer::getScreenshotFromVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    /*
    QImage img = vncWdg->getScreenCapture();
    // WARNING: used %1%2%3.snapshot template,
    // because filter will added to tail the template
    // after last dot.
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Save Image to",
                QString("%1%2%3_%4_%5.snapshot")
                    .arg(QDir::homePath())
                    .arg(QDir::separator())
                    .arg(guestName)
                    .arg(QDate::currentDate().toString("dd.MM.yyyy"))
                    .arg(QTime::currentTime().toString()),
                "Images (*.png)");
    if ( !fileName.isNull() ) {
        img.save(fileName, "png");
    };
    */
}
void VNC_Viewer::copyFilesToVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, "Copy files to Guest", "~");
    //vncWdg->fileCopyAsync(fileNames);
}
void VNC_Viewer::copyToClipboardFromVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    //vncWdg->copyClipboardDataFromGuest();
}
void VNC_Viewer::pasteClipboardToVirtDomain()
{
    if ( nullptr==vncWdg ) return;
    QString _text = QApplication::clipboard()->text(QClipboard::Clipboard);
    QImage _image = QApplication::clipboard()->image(QClipboard::Clipboard);
    qDebug()<<"copy:"<<_text<<_image.isNull()<<";";
    if ( !_text.isEmpty() ) {
        //vncWdg->sendClipboardDataToGuest(
        //            VD_AGENT_CLIPBOARD_UTF8_TEXT,
        //            (const uchar*)_text.toUtf8().data(),
        //            _text.size());
    };
    if ( !_image.isNull() ) {
        /*
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
            qDebug()<<"png";
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
        */
        //vncWdg->sendClipboardDataToGuest(
        //            VD_AGENT_CLIPBOARD_IMAGE_PNG,
        //            _image.constBits(),
        //            _image.byteCount());
    };
}

/* private slots */
void VNC_Viewer::initVNCWidget()
{
    vncWdg = new MachineView(this);
    setCentralWidget(vncWdg);
    /*
    connect(vncWdg, SIGNAL(displayResized(const QSize&)),
            SLOT(resizeViewer(const QSize&)));
    connect(vncWdg, SIGNAL(downloaded(int,int)),
            vm_stateWdg, SLOT(setDownloadProcessValue(int,int)));
    connect(vncWdg, SIGNAL(displayChannelChanged(bool)),
            vm_stateWdg, SLOT(changeDisplayState(bool)));
    connect(vncWdg, SIGNAL(cursorChannelChanged(bool)),
            vm_stateWdg, SLOT(changeMouseState(bool)));
    connect(vncWdg, SIGNAL(inputsChannelChanged(bool)),
            vm_stateWdg, SLOT(changeKeyboardState(bool)));
    connect(vncWdg, SIGNAL(usbredirChannelChanged(bool)),
            vm_stateWdg, SLOT(changeUsbredirState(bool)));
    connect(vncWdg, SIGNAL(smartcardChannelChanged(bool)),
            vm_stateWdg, SLOT(changeSmartcardState(bool)));
    connect(vncWdg, SIGNAL(webdavChannelChanged(bool)),
            vm_stateWdg, SLOT(changeWebDAVState(bool)));
    connect(vncWdg, SIGNAL(playbackChannelChanged(bool)),
            vm_stateWdg, SLOT(changePlaybackState(bool)));
    connect(vncWdg, SIGNAL(recordChannelChanged(bool)),
            vm_stateWdg, SLOT(changeRecordState(bool)));
    connect(vm_stateWdg, SIGNAL(showUsbDevWidget()),
            vncWdg, SLOT(showUsbDevWidget()));
    connect(vm_stateWdg, SIGNAL(showSmartCardWidget()),
            vncWdg, SLOT(showSmartCardWidget()));
    connect(vm_stateWdg, SIGNAL(transformationMode(Qt::TransformationMode)),
            vncWdg, SLOT(setTransformationMode(Qt::TransformationMode)));
    connect(vncWdg, SIGNAL(errMsg(QString&)),
            this, SLOT(sendErrMsg(QString&)));
    connect(vncWdg, SIGNAL(clipboardsReleased(bool)),
            viewerToolBar, SLOT(changeCopypasteState(bool)));
    */

    QSize around_size = getWidgetSizeAroundDisplay();
    qDebug()<<"address:"<<addr<<port;
    vncWdg->Set_VNC_URL(addr, port);
    vncWdg->initView();
    vncWdg->newViewSize(around_size.width(), around_size.height());
}

void VNC_Viewer::timerEvent(QTimerEvent *ev)
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

void VNC_Viewer::resizeViewer(const QSize &size)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    resize(size+around_size);
}

void VNC_Viewer::fullScreenTriggered()
{
    if (isFullScreen())
        setWindowState(Qt::WindowNoState);
    else
        setWindowState(Qt::WindowFullScreen);
}

void VNC_Viewer::resizeEvent(QResizeEvent *ev)
{
    QSize around_size = getWidgetSizeAroundDisplay();
    if ( nullptr!=vncWdg ) {
        vncWdg->newViewSize(
                    ev->size().width()-around_size.width(),
                    ev->size().height()-around_size.height());
    };
}

QSize VNC_Viewer::getWidgetSizeAroundDisplay()
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
