
/* http://www.spice-space.org/page/Whiteboard/AgentProtocol#Clipboard
 * To do so, the guest agent and the client play a symmetric role:
 * they can both claim ownership (GRAB), RELEASE ownership,
 * REQUEST clipboard data and send CLIPBOARD data. For example,
 * the GRAB message is sent after receiving a system notification of clipboard data
 * available after a Copy operation in some application.
 * When the clipboard is emptied, the grab must be RELEASEd. The other side can REQUEST
 * the data while the GRAB is active, and should expect a CLIPBOARD reply with the data.
 * Important:
 * If a GRAB message has been sent and is currently active,
 * then a successive GRAB message is received from the peer,
 * no RELEASE message should be sent to the peer for the previous active grab.
 * It has been implicitly released by the peer. Sending an extra RELEASE message
 * would only confuse the peer.
 * ------------------------------------------------------------------------------------
 * Then will implemented the copy/paste cross blocking.
 */

#include <spice-client.h>
#include <spice/vd_agent.h>
#include "qspice-widget.h"
#include "qspice-smartcard-widget.h"
#include "qspice-usbdevice-widget.h"

#include <QApplication>
#include <QMouseEvent>
#include <QClipboard>
#include <QTime>
#include <QDate>
#include <QFileDialog>

#define MARGIN 5

QSpiceWidget::QSpiceWidget(QWidget *parent) :
    QWidget(parent)
{
    spiceSession = new QSpiceSession(this);
    display = nullptr;
    inputs = nullptr;
    cursor = nullptr;
    smartcard = nullptr;
    usbredir = nullptr;
    webdav = nullptr;
    playback = nullptr;
    usbDevManager = nullptr;
    smartcardManager = nullptr;
    spiceAudio = nullptr;
    _width = 0;
    _height = 0;
    init_h = 0;
    init_w = 0;
    zoom = 1.0;

    tr_mode = Qt::SmoothTransformation;
    img = nullptr;
    m_Image = new QLabel(this);
    m_Image->setAlignment(Qt::AlignTop | Qt::AlignJustify);
    m_Image->setContentsMargins(0,0,0,0);
    scrolled = new QScrollArea(this);
    scrolled->setWidgetResizable(true);
    scrolled->setContentsMargins(0,0,0,0);
    scrolled->setWidget(m_Image);

    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(scrolled);
    commonLayout->setContentsMargins(0,0,0,0);
    setLayout(commonLayout);

    resizeTimer.setSingleShot( true );
    connect(&resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()));

    connect(spiceSession, SIGNAL(channelInited(QSpiceChannel*)),
            SLOT(setChannel(QSpiceChannel*)));
    connect(spiceSession, SIGNAL(channelDestroyed(int)),
            SLOT(obstructChannel(int)));
    m_Image->setMouseTracking(true);
    m_Image->setFocusPolicy(Qt::StrongFocus);
    m_Image->installEventFilter( this );
    setContentsMargins(MARGIN,MARGIN,MARGIN,MARGIN);
}
QSpiceWidget::~QSpiceWidget()
{
    disconnectFromSpiceSource();
}

bool QSpiceWidget::connectToSpiceSource(QString &uri)
{
    spiceSession->setUri(uri);
    return spiceSession->openSession();
}

void QSpiceWidget::disconnectFromSpiceSource()
{
    spiceSession->closeSession();
}

void QSpiceWidget::sendKeySequience(Qt::Key key)
{
    if ( nullptr!=inputs ) {
        // inputs->inputsKeyPressAndRelease()
        // does not give the desired effect
        inputs->inputsQKeyPress(key);
        inputs->inputsQKeyRelease(key);
    };
}

void QSpiceWidget::fileCopyAsync(QStringList &fileNames)
{
    main->fileCopyAsync(fileNames);
}

void QSpiceWidget::copyClipboardDataFromGuest()
{
    emit clipboardsReleased(false);
    main->guestClipboardSelectionRequest();
    emit clipboardsReleased(true);
}

void QSpiceWidget::sendClipboardDataToGuest(quint32 type, const uchar *_data, size_t _size)
{
    emit clipboardsReleased(false);
    main->clipboardSelectionNotify(type, _data, _size);
    emit clipboardsReleased(true);
}

/* private slots */
void QSpiceWidget::setChannel(QSpiceChannel *channel)
{
    QSpiceMainChannel * _main =
            dynamic_cast<QSpiceMainChannel *>(channel);
    if (_main)
    {
        main = _main;
        connect(main, SIGNAL(agentUpdated()),
                SLOT(mainAgentUpdate()));
        connect(main, SIGNAL(clipboardSelection(uint,void*,uint)),
                SLOT(mainClipboardSelection(uint,void*,uint)));
        connect(main, SIGNAL(clipboardSelectionGrabbed(uint,void*,uint)),
                SLOT(clipboardSelectionGrab()));
        connect(main, SIGNAL(clipboardSelectionReleased(uint)),
                SLOT(guestClipboardSelectionRelease(uint)));
        connect(main, SIGNAL(clipboardSelectionRequested(uint,uint)),
                SLOT(clientClipboardSelectionRequest(uint,uint)));
        connect(main, SIGNAL(mouseUpdated()),
                SLOT(mainMouseUpdate()));
        connect(main, SIGNAL(downloaded(int,int)),
                this, SIGNAL(downloaded(int,int)));
        main->connectToChannel();
        return;
    }

    QSpiceDisplayChannel *_display =
            dynamic_cast<QSpiceDisplayChannel *>(channel);
    if (_display)
    {
        display = _display;
        connect(display, SIGNAL(displayPrimaryCreated(int,int,int,int,int,void*)),
                SLOT(displayPrimaryCreate(int,int,int,int,int,void*)));
        connect(display, SIGNAL(displayInvalidated(int,int,int,int)),
                SLOT(displayInvalidate(int,int,int,int)));
        connect(display, SIGNAL(displayPrimaryDestroyed()),
                SLOT(displayPrimaryDestroy()));
        connect(display, SIGNAL(displayMarked(int)),
                SLOT(displayMark(int)));

        bool online = display->connectToChannel();
        emit displayChannelChanged(online);
        return;
    }

    QSpiceInputsChannel * _inputs =
            dynamic_cast<QSpiceInputsChannel *>(channel);
    if (_inputs)
    {
        inputs = _inputs;
        bool online = inputs->connectToChannel();
        emit inputsChannelChanged(online);
        return;
    }

    QSpiceCursorChannel * _cursor =
            dynamic_cast<QSpiceCursorChannel *>(channel);
    if (_cursor)
    {
        cursor = _cursor;
        connect(cursor, SIGNAL(cursorData(int,int,int,int,void*)),
                SLOT(setClientCursor(int,int,int,int,void*)));
        bool online = cursor->connectToChannel();
        emit cursorChannelChanged(online);
        return;
    }

    QSpiceSmartcardChannel * _smartcard =
            dynamic_cast<QSpiceSmartcardChannel *>(channel);
    if (_smartcard)
    {
        smartcard = _smartcard;
        bool online = false;
#if WITH_LIBCACARD
        online = smartcard->connectToChannel();
        qDebug()<<"WITH_LIBCACARD";
#else
        qDebug()<<"NOT WITH_LIBCACARD";
#endif
        if ( online ) {
            smartcardManager = new QSpiceSmartcardManager(this);
            if ( smartcardManager ) {
                connect(smartcardManager, SIGNAL(cardInserted(QString&)),
                        this, SLOT(cardInserted(QString&)));
                connect(smartcardManager, SIGNAL(cardRemoved(QString&)),
                        this, SLOT(cardRemoved(QString&)));
                connect(smartcardManager, SIGNAL(readerAdded(QString&)),
                        this, SLOT(readerAdded(QString&)));
                connect(smartcardManager, SIGNAL(readerRemoved(QString&)),
                        this, SLOT(readerRemoved(QString&)));
            };
        };
        emit smartcardChannelChanged(online);
        return;
    }

    QSpiceUSBRedirChannel * _usbredir =
            dynamic_cast<QSpiceUSBRedirChannel *>(channel);
    if (_usbredir)
    {
        usbredir = _usbredir;
        bool online = usbredir->connectToChannel();
        if ( online ) {
            usbDevManager = new QSpiceUsbDeviceManager(this, spiceSession);
            if ( usbDevManager ) {
                connect(usbDevManager, SIGNAL(autoConnectFailed(QString&,QString&)),
                        this, SLOT(usbDevAutoConnectFailed(QString&,QString&)));
                connect(usbDevManager, SIGNAL(deviceAdded(QString&)),
                        this, SLOT(usbDevAdded(QString&)));
                connect(usbDevManager, SIGNAL(deviceInfo(QString&,QString&)),
                        this, SLOT(usbDevError(QString&,QString&)));
                connect(usbDevManager, SIGNAL(deviceRemoved(QString&)),
                        this, SLOT(usbDevRemoved(QString&)));
            };
        };
        emit usbredirChannelChanged(online);
        return;
    }

    QSpiceWebDAVChannel * _webdav =
            dynamic_cast<QSpiceWebDAVChannel *>(channel);
    if (_webdav)
    {
        webdav = _webdav;
        bool online = webdav->connectToChannel();
        emit webdavChannelChanged(online);
        return;
    }

    QSpicePlaybackChannel * _playback =
            dynamic_cast<QSpicePlaybackChannel *>(channel);
    if (_playback)
    {
        playback = _playback;
        bool online = false;
#if USE_SPICE_AUDIO
        online = playback->connectToChannel();
        qDebug()<<"USE_SPICE_AUDIO";
#else
        qDebug()<<"NOT USE_SPICE_AUDIO";
#endif
        if ( online && !spiceAudio ) {
            spiceAudio = new QSpiceAudio(
                        this,
                        static_cast<SpiceSession*>(spiceSession->gobject));
            //qDebug()<<"spiceAudio is associated:"<<spiceAudio->isAssociated();
            if ( spiceAudio->isAssociated() ) {
                // reserved for some work
            };
        };
        emit playbackChannelChanged(online);
        return;
    }

    QSpiceRecordChannel * _record =
            dynamic_cast<QSpiceRecordChannel *>(channel);
    if (_record)
    {
        record = _record;
        bool online = false;
#if USE_SPICE_AUDIO
        online = record->connectToChannel();
        qDebug()<<"USE_SPICE_AUDIO";
#else
        qDebug()<<"NOT USE_SPICE_AUDIO";
#endif
        if ( online && !spiceAudio ) {
            spiceAudio = new QSpiceAudio(
                        this,
                        static_cast<SpiceSession*>(spiceSession->gobject));
            //qDebug()<<"spiceAudio is associated:"<<spiceAudio->isAssociated();
            if ( spiceAudio->isAssociated() ) {
                // reserved for some work
            };
        };
        emit recordChannelChanged(online);
        return;
    }

}

void QSpiceWidget::obstructChannel(int channelType)
{
    switch(channelType)
    {
    case SPICE_CHANNEL_MAIN:
        delete main;
        main = nullptr;
        break;

    case SPICE_CHANNEL_DISPLAY:
        delete display;
        display = nullptr;
        emit displayChannelChanged(false);
        break;

    case SPICE_CHANNEL_INPUTS:
        delete inputs;
        inputs = nullptr;
        emit inputsChannelChanged(false);
        break;

    case SPICE_CHANNEL_CURSOR:
        m_Image->setCursor(Qt::ArrowCursor);
        delete cursor;
        cursor = nullptr;
        emit cursorChannelChanged(false);
        break;

    case SPICE_CHANNEL_SMARTCARD:
        if (smartcardManager) {
            smartcardManager->deleteLater();
        };
        delete smartcard;
        smartcard = nullptr;
        emit smartcardChannelChanged(false);
        break;

    case SPICE_CHANNEL_USBREDIR:
        if ( usbDevManager ) {
            usbDevManager->deleteLater();
        };
        delete usbredir;
        usbredir = nullptr;
        emit usbredirChannelChanged(false);
        break;

    case SPICE_CHANNEL_WEBDAV:
        delete webdav;
        webdav = nullptr;
        emit webdavChannelChanged(false);
        break;

    case SPICE_CHANNEL_PLAYBACK:
        delete playback;
        playback = nullptr;
        emit playbackChannelChanged(false);
        if ( spiceAudio ) {
            spiceAudio->deleteLater();
        };
        break;

    case SPICE_CHANNEL_RECORD:
        delete record;
        record = nullptr;
        emit recordChannelChanged(false);
        if ( spiceAudio ) {
            spiceAudio->deleteLater();
        };
        break;

    default:
        break;
    }

}

void QSpiceWidget::mainAgentUpdate()
{
    //qDebug()<<"main: AgentUpdate";
}

void QSpiceWidget::mainClipboardSelection(uint type, void *_data, uint _size)
{
    //qDebug()<<"main: ClipboardSelection";
    QImage _img;
    bool res;
    switch (type) {
    case VD_AGENT_CLIPBOARD_NONE:
        break;
    case VD_AGENT_CLIPBOARD_UTF8_TEXT:
        QApplication::clipboard()->setText(
                    QString::fromUtf8((char*)_data, _size));
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_PNG:
        res = _img.loadFromData((uchar*)_data, _size, "PNG");
        if (res) QApplication::clipboard()->setImage(_img);
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_BMP:
        res = _img.loadFromData((uchar*)_data, _size, "BMP");
        if (res) QApplication::clipboard()->setImage(_img);
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_JPG:
        res = _img.loadFromData((uchar*)_data, _size, "JPG");
        if (res) QApplication::clipboard()->setImage(_img);
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_TIFF:
        res = _img.loadFromData((uchar*)_data, _size, "TIFF");
        if (res) QApplication::clipboard()->setImage(_img);
        break;
    default:
        break;
    };
}

void QSpiceWidget::clipboardSelectionGrab()
{
    qDebug()<<"main: ClipboardSelectionGrab";
}

void QSpiceWidget::guestClipboardSelectionRelease(uint selection)
{
    switch (selection) {
    case VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD:
        qDebug()<<"guestClipboard_CLIPBOARDSelectionRelease";
        break;
    case VD_AGENT_CLIPBOARD_SELECTION_PRIMARY:
        qDebug()<<"guestClipboard_PRIMARYSelectionRelease";
        break;
    case VD_AGENT_CLIPBOARD_SELECTION_SECONDARY:
        qDebug()<<"guestClipboard_SECONDARYSelectionRelease";
        break;
    default:
        break;
    };
    emit clipboardsReleased(true);
}

void QSpiceWidget::clientClipboardSelectionRequest(uint selection, uint type)
{
    QString dataType;
    switch (type) {
    case VD_AGENT_CLIPBOARD_NONE:
        dataType.append("None");
        break;
    case VD_AGENT_CLIPBOARD_UTF8_TEXT:
        dataType.append("UTF8_TEXT");
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_PNG:
        dataType.append("IMAGE_PNG");
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_BMP:
        dataType.append("IMAGE_BMP");
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_JPG:
        dataType.append("IMAGE_JPG");
        break;
    case VD_AGENT_CLIPBOARD_IMAGE_TIFF:
        dataType.append("IMAGE_TIFF");
        break;
    default:
        break;
    };
    switch (selection) {
    case VD_AGENT_CLIPBOARD_SELECTION_CLIPBOARD:
        qDebug()<<"clientClipboard_CLIPBOARDSelectionRequest"<<dataType;
        break;
    case VD_AGENT_CLIPBOARD_SELECTION_PRIMARY:
        qDebug()<<"clientClipboard_PRIMARYSelectionRequest"<<dataType;
        break;
    case VD_AGENT_CLIPBOARD_SELECTION_SECONDARY:
        qDebug()<<"clientClipboard_SECONDARYSelectionRequest"<<dataType;
        break;
    default:
        break;
    }
}

void QSpiceWidget::mainMouseUpdate()
{
    //qDebug()<<"main: MouseUpdate";
}

void QSpiceWidget::usbDevAutoConnectFailed(QString &dev, QString &err)
{
    QString _msg = QString("<font color='red'>UsbDevAutoConnectFailed</font>: %1 : %2")
            .arg(dev).arg(err);
    emit errMsg(_msg);
    //qDebug()<<"usbDevAutoConnectFailed:"<< dev<< err;
}

void QSpiceWidget::usbDevAdded(QString &dev)
{
    QString _msg = QString("<font color='blue'>UsbDevAdded</font>: %1")
            .arg(dev);
    emit errMsg(_msg);
    //qDebug()<<"usbDevAdded:"<< dev;
}

void QSpiceWidget::usbDevError(QString &dev, QString &err)
{
    QString _msg = QString("<font color='blue'>UsbDevInfo</font>: %1 : %2")
            .arg(dev).arg(err);
    emit errMsg(_msg);
    //qDebug()<<"usbDevError"<< dev<< err;
}

void QSpiceWidget::usbDevRemoved(QString &dev)
{
    QString _msg = QString("<font color='blue'>UsbDevRemoved</font>: %1")
            .arg(dev);
    emit errMsg(_msg);
    //qDebug()<<"usbDevRemoved:"<< dev;
}

void QSpiceWidget::cardInserted(QString &_vcard)
{
    qDebug()<<"cardInserted"<<_vcard;
}

void QSpiceWidget::cardRemoved(QString &_vcard)
{
    qDebug()<<"cardRemoved"<<_vcard;
}

void QSpiceWidget::readerAdded(QString &_reader)
{
    qDebug()<<"readerAdded"<<_reader;
}

void QSpiceWidget::readerRemoved(QString &_reader)
{
    qDebug()<<"readerRemoved"<<_reader;
}

void QSpiceWidget::displayPrimaryCreate(
     int                 format,
     int                 width,
     int                 height,
     int                 stride,
     int                 shmid,
     void*               imgdata)
{
    Q_UNUSED(shmid)
    m_Image->setUpdatesEnabled(false);

    //qDebug() << "Display Create(" << width << ", " << height << ")";
    init_h = height; init_w = width;
    QImage::Format _format = QImage::Format_Invalid;

    if (img) {
        delete img;
        img = nullptr;
    };
    switch(format) {
    case SPICE_SURFACE_FMT_32_xRGB:
        _format = QImage::Format_RGB32;
        break;

    case SPICE_SURFACE_FMT_16_555:
        _format = QImage::Format_RGB555;
        break;

    default:
        qDebug() << "Unknown display format " << _format;
    };
    if ( _format!=QImage::Format_Invalid ) {
        img = new QImage(
                    static_cast<uchar*>(imgdata),
                    init_w,
                    init_h,
                    stride,
                    _format);
    };

    if (img) {
        m_Image->setUpdatesEnabled(true);
        QPixmap pix = QPixmap::fromImage(*img);
        m_Image->setPixmap(pix);
        QSize _size(init_w+2*MARGIN, init_h+2*MARGIN+4);
        //qDebug()<<_size<<"emit";
        emit displayResized(_size);
    }
}

void QSpiceWidget::displayInvalidate(
    int                 x,
    int                 y,
    int                 width,
    int                 height)
{
    Q_UNUSED(width)
    Q_UNUSED(height)
    Q_UNUSED(x)
    Q_UNUSED(y)
    //qDebug()<<"displayInvalidate"<<x<<y<<width<<height<<":"
    //       <<x*zoom<<y*zoom<<width*zoom<<height*zoom;
    //if (img==nullptr) return;
    //QPixmap pix = QPixmap::fromImage(
    //            img->scaled(
    //                _width,
    //                _height,
    //                Qt::KeepAspectRatio,
    //                tr_mode));
    //zoom = init_w/(qreal)pix.width();
    //m_Image->setPixmap(pix);
    // TODO: implement the update the sent rectangle only
    m_Image->update();
}

void QSpiceWidget::displayPrimaryDestroy()
{
    //qDebug() << "Display Destroyed";
    m_Image->setPixmap(QPixmap());
    m_Image->setUpdatesEnabled(false);
}

void QSpiceWidget::displayMark(int mark)
{
    //qDebug() << "Display Mark " << mark;
    m_Image->setUpdatesEnabled(mark != 0);
}

void QSpiceWidget::setClientCursor(
        int                width,
        int                height,
        int                hot_x,
        int                hot_y,
        void*              rgba)
{
    QImage c_img((uchar*) rgba, width, height, QImage::Format_ARGB32);
    QPixmap pix = QPixmap::fromImage(c_img);
    QCursor c(pix, hot_x, hot_y);
    m_Image->setCursor(c);
}

void QSpiceWidget::reloadUsbDevList(void *obj)
{
    QSpiceUsbDeviceWidget *usbDevWdg =
            static_cast<QSpiceUsbDeviceWidget*>(obj);
    if ( usbDevWdg ) {
        usbDevWdg->setEnabled(false);
        usbDevWdg->clearList();
        QStringList _devList =
                usbDevManager->spiceUsbDeviceManager_get_devices();
        foreach (QString _dev, _devList) {
            QString _name, _desc;
            QStringList _split = _dev.split("<||>");
            _name = _split.first();
            _desc = _split.last();
            QString _id = QString("%1 %2").arg(_name).arg(_desc);
            bool connected = usbDevManager->
                    spiceUsbDeviceManager_is_device_connected(_id);
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(_name);
            item->setData(Qt::UserRole, _desc);
            item->setCheckState(connected? Qt::Checked:Qt::Unchecked);
            usbDevWdg->addItem(item);
        };
        usbDevWdg->setEnabled(true);
    };
}

void QSpiceWidget::reloadSmartcardList(void *obj)
{
    QSpiceSmartcardWidget *smartcardWdg =
            static_cast<QSpiceSmartcardWidget*>(obj);
    if ( smartcardWdg ) {
        smartcardWdg->setEnabled(false);
        smartcardWdg->clearList();
        QStringList _cardList = smartcardManager->
                spiceSmartcardManager_get_readers();
        foreach (QString _card, _cardList) {
            QListWidgetItem *item = new QListWidgetItem();
            item->setText(_card);
            item->setCheckState(Qt::Checked);
            smartcardWdg->addItem(item);
        };
        smartcardWdg->setEnabled(true);
    };
}

int QtButtonToSpice(QMouseEvent *ev)
{
    switch (ev->button())
    {
    case Qt::LeftButton:
        return SPICE_MOUSE_BUTTON_LEFT;

    case Qt::MiddleButton:
        return SPICE_MOUSE_BUTTON_MIDDLE;

    case Qt::RightButton:
        return SPICE_MOUSE_BUTTON_RIGHT;

    default:
        return SPICE_MOUSE_BUTTON_INVALID;
    }
}

int QtButtonsMaskToSpice(Qt::MouseButtons buttons)
{
    int mask = 0;
    if (buttons & Qt::LeftButton)
        mask |= SPICE_MOUSE_BUTTON_MASK_LEFT;

    if (buttons & Qt::MiddleButton)
        mask |= SPICE_MOUSE_BUTTON_MASK_MIDDLE;

    if (buttons & Qt::RightButton)
        mask |= SPICE_MOUSE_BUTTON_MASK_RIGHT;

    return mask;
}

int QtButtonsMaskToSpice(QMouseEvent *ev)
{
    return QtButtonsMaskToSpice(ev->buttons());
}

bool QSpiceWidget::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    if (! (display && inputs))
        return false;

    if (event->type() == QEvent::MouseMove )
    {
        QMouseEvent *ev = static_cast<QMouseEvent*>(event);
        //qDebug()<<ev->x()<<ev->y()<<":"
        //<<ev->x()*zoom<<ev->y()*zoom<<":"<<zoom;
        inputs->inputsPosition(
                    //ev->x()*zoom,
                    //ev->y()*zoom,
                    ev->x(),
                    ev->y(),
                    display->getId(),
                    QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ev = static_cast<QMouseEvent*>(event);
        inputs->inputsButtonPress(
                    QtButtonToSpice(ev), QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *ev = static_cast<QMouseEvent*>(event);
        inputs->inputsButtonRelease(
                    QtButtonToSpice(ev), QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(event);
        inputs->inputsQKeyPress(ev->key());
        return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = static_cast<QKeyEvent*>(event);
        inputs->inputsQKeyRelease(ev->key());
        return true;
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        inputs->inputsButtonPress(
                    SPICE_MOUSE_BUTTON_LEFT,
                    SPICE_MOUSE_BUTTON_MASK_LEFT);
        inputs->inputsButtonRelease(
                    SPICE_MOUSE_BUTTON_LEFT,
                    0);
        return true;
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *ev = static_cast<QWheelEvent*>(event);
        if (ev->delta() > 0)
        {
            inputs->inputsButtonPress(
                        SPICE_MOUSE_BUTTON_UP,
                        QtButtonsMaskToSpice(
                            QApplication::mouseButtons()));
            inputs->inputsButtonRelease(
                        SPICE_MOUSE_BUTTON_UP,
                        QtButtonsMaskToSpice(
                            QApplication::mouseButtons()));
        }
        else
        {
            inputs->inputsButtonPress(
                        SPICE_MOUSE_BUTTON_DOWN,
                        QtButtonsMaskToSpice(
                            QApplication::mouseButtons()));
            inputs->inputsButtonRelease(
                        SPICE_MOUSE_BUTTON_DOWN,
                        QtButtonsMaskToSpice(
                            QApplication::mouseButtons()));

        }
        ev->accept();
        return true;
    }


    return false;

}

void QSpiceWidget::resizeEvent ( QResizeEvent * event )
{
    Q_UNUSED(event)

    if ( main && display )
        resizeTimer.start(500);
}

void QSpiceWidget::resizeDone()
{
    if ( main && display ) {
        main->setDisplay(
                    display->getId(),
                    0,
                    0,
                    _width,
                    _height);
        //main->setDisplayEnabled(display->getId(), true);
        //qDebug()<<"configured"<<
        main->sendMonitorConfig();
    }

}

/* public slots */
void QSpiceWidget::setGuestName(QString &_name)
{
    guestName = _name;
}

void QSpiceWidget::setNewSize(int _w, int _h)
{
    _width  = _w-2*MARGIN;
    _height = _h-2*MARGIN-4;
    resizeDone();
}
void QSpiceWidget::updateSize(int _w, int _h)
{
    _width  = _w-2*MARGIN;
    _height = _h-2*MARGIN-4;
    if ( main && display ) {
         // update equal TRUE for update after 1 sec
         // without further changes, because it used
         // at parent widget resizes
         main->updateDisplay(
                     display->getId(),
                     0,
                     0,
                     _width,
                     _height,
                     true);
    };
    displayInvalidate(0,0,0,0);
}

void QSpiceWidget::showUsbDevWidget()
{
    QSpiceUsbDeviceWidget *usbDevWdg =
            new QSpiceUsbDeviceWidget(this);
    if ( usbDevManager ) {
        connect(usbDevManager, SIGNAL(deviceAdded(QString&)),
                usbDevWdg, SLOT(addDevice(QString&)));
        connect(usbDevManager, SIGNAL(deviceRemoved(QString&)),
                usbDevWdg, SLOT(removeDevice(QString&)));
        connect(usbDevWdg, SIGNAL(connectDevice(QString&)),
                usbDevManager, SLOT(spiceUsbDeviceManager_connect_device(QString&)));
        connect(usbDevWdg, SIGNAL(disconnectDevice(QString&)),
                usbDevManager, SLOT(spiceUsbDeviceManager_disconnect_device(QString&)));
        connect(usbDevWdg, SIGNAL(devicesChanged(void*)),
                this, SLOT(reloadUsbDevList(void*)));
        reloadUsbDevList(usbDevWdg);
    };
    usbDevWdg->exec();
    usbDevWdg->deleteLater();
}

void QSpiceWidget::showSmartCardWidget()
{
    QSpiceSmartcardWidget *smartcardWdg =
            new QSpiceSmartcardWidget(this);
    if ( smartcardManager ) {
        connect(smartcardManager, SIGNAL(cardInserted(QString&)),
                smartcardWdg, SLOT(addCard(QString&)));
        connect(smartcardManager, SIGNAL(cardRemoved(QString&)),
                smartcardWdg, SLOT(removeCard(QString&)));
        //connect(smartcardWdg, SIGNAL(connectCard(QString&)),
        //        smartcardManager, SLOT(spiceSmartcardReader_insert_card(QString&)));
        //connect(smartcardWdg, SIGNAL(disconnectCard(QString&)),
        //        smartcardManager, SLOT(spiceSmartcardReader_remove_card(QString&)));
        connect(smartcardWdg, SIGNAL(cardsChanged(void*)),
                this, SLOT(reloadSmartcardList(void*)));
        reloadSmartcardList(smartcardWdg);
    };
    smartcardWdg->exec();
    smartcardWdg->deleteLater();
}

void QSpiceWidget::getScreenshot()
{
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
    if ( !fileName.isNull() && img!=nullptr ) {
        img->save(fileName, "png");
    };
}

void QSpiceWidget::setTransformationMode(Qt::TransformationMode _mode)
{
    tr_mode = _mode;
}
