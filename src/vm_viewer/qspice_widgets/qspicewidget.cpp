#include "qspicewidget.h"

#include <QDebug>
#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>

QSpiceWidget::QSpiceWidget(QWidget *parent) :
    QScrollArea(parent)
{
    spiceSession = new QSpiceSession(this);
    display = NULL;
    inputs = NULL;

    m_Image = new QLabel(this);
    m_Image->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    setWidget(m_Image);
    setFrameStyle(QFrame::NoFrame);

    resizeTimer.setSingleShot( true );
    connect(&resizeTimer, SIGNAL(timeout()), SLOT(resizeDone()));

    connect(spiceSession, SIGNAL(channelNew(QSpiceChannel*)),
            SLOT(ChannelNew(QSpiceChannel*)));
    m_Image->setMouseTracking(true);
    m_Image->setFocusPolicy(Qt::StrongFocus);
    m_Image->installEventFilter( this );
}

QSpiceWidget::~QSpiceWidget()
{
    delete spiceSession;
}

bool QSpiceWidget::Connect(QString uri)
{
    spiceSession->setUri(uri);
    return spiceSession->Connect();
}
void QSpiceWidget::Disconnect()
{
    spiceSession->Disconnect();
}
void QSpiceWidget::SendKeySequience(Qt::Key key)
{
    if ( NULL!=inputs ) {
        // inputs->inputsKeyPressAndRelease()
        // does not give the desired effect
        inputs->inputsQKeyPress(key);
        inputs->inputsQKeyRelease(key);
    };
}

void QSpiceWidget::ChannelNew(QSpiceChannel *channel)
{
    QSpiceMainChannel * _main = dynamic_cast<QSpiceMainChannel *>(channel);
    if (_main)
    {
        main = _main;
        connect(main, SIGNAL(channelDestroyed()), SLOT(channelDestroyed()));
        main->Connect();
        return;
    }

    QSpiceDisplayChannel *_display = dynamic_cast<QSpiceDisplayChannel *>(channel);
    if (_display)
    {
        display = _display;
        connect(display, SIGNAL(displayPrimaryCreate(int,int,int,int,int,void*)),
                SLOT(displayPrimaryCreate(int,int,int,int,int,void*)));
        connect(display, SIGNAL(displayInvalidate(int,int,int,int)),
                SLOT(displayInvalidate(int,int,int,int)));
        connect(display, SIGNAL(displayPrimaryDestroy()),
                SLOT(displayPrimaryDestroy()));
        connect(display, SIGNAL(displayMark(int)),
                SLOT(displayMark(int)));
        connect(display, SIGNAL(channelDestroyed()),
                SLOT(displayPrimaryDestroy()));

        display->Connect();
        return;
    }

    QSpiceInputsChannel * _inputs = dynamic_cast<QSpiceInputsChannel *>(channel);
    if (_inputs)
    {
        inputs = _inputs;
        connect(inputs, SIGNAL(channelDestroyed()), SLOT(channelDestroyed()));
        inputs->Connect();
        return;
    }

    QSpiceCursorChannel * _cursor = dynamic_cast<QSpiceCursorChannel *>(channel);
    if (_cursor)
    {
        cursor = _cursor;
        connect(cursor, SIGNAL(channelDestroyed()), SLOT(channelDestroyed()));
        connect(cursor, SIGNAL(cursorSet(int,int,int,int,void*)),
                SLOT(cursorSet(int,int,int,int,void*)));
        cursor->Connect();
        return;
    }

}

void QSpiceWidget::channelDestroyed()
{
    if (QObject::sender() == main)
        main = NULL;
    else if (QObject::sender() == inputs)
        inputs = NULL;
    else if (QObject::sender() == cursor)
    {
        m_Image->setCursor(Qt::ArrowCursor);
        cursor = NULL;
    }
}


void QSpiceWidget::displayPrimaryCreate(
     int                 format,
     int                 width,
     int                 height,
     int                 stride,
     int                 shmid,
     void  *             imgdata)
{
    Q_UNUSED(shmid);
    m_Image->setUpdatesEnabled(false);

    //qDebug() << "Display Create(" << width << ", " << height << ")";

    QImage *img = NULL;
    switch(format)
    {
    case SPICE_SURFACE_FMT_32_xRGB:
        img = new QImage(
                    static_cast<uchar *>(imgdata),
                    width,
                    height,
                    stride,
                    QImage::Format_RGB32);
        break;

    case SPICE_SURFACE_FMT_16_555:
        img = new QImage(
                    static_cast<uchar *>(imgdata),
                    width,
                    height,
                    stride,
                    QImage::Format_RGB555);
        break;

    default:
        qDebug() << "Unknown display format " << format;
    }

    if (img)
    {
        m_Image->setUpdatesEnabled(false);
        QPixmap pix =  QPixmap::fromImage(*img);
        delete img;
        m_Image->setPixmap(pix);
        //m_Image->setFixedSize(width, height);
        //resize(m_Image->frameSize());
        //emit DisplayResize(m_Image->frameSize());
        QSize _size(width, height);
        resize(_size);
        emit DisplayResize(_size);
    }
}


void QSpiceWidget::displayInvalidate(
    int                 x,
    int                 y,
    int                 width,
    int                 height)
{
    m_Image->update(x, y, width, height);
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

void QSpiceWidget::cursorSet(
        int                width,
        int                height,
        int                hot_x,
        int                hot_y,
        void *             rgba)
{
    QImage img((uchar *) rgba, width, height, QImage::Format_ARGB32);
    QPixmap pix = QPixmap::fromImage(img);
    QCursor c(pix, hot_x, hot_y);

    m_Image->setCursor(c);


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
    Q_UNUSED(object);

    if (! (display && inputs))
        return false;

    if (event->type() == QEvent::MouseMove )
    {
        QMouseEvent *ev = (QMouseEvent *) event;
        inputs->inputsPosition(ev->x(), ev->y(), display->getId(), QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ev = (QMouseEvent *) event;
        inputs->inputsButtonPress(QtButtonToSpice(ev), QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *ev = (QMouseEvent *) event;
        inputs->inputsButtonRelease(QtButtonToSpice(ev), QtButtonsMaskToSpice(ev));
        return true;
    }
    else if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *ev = (QKeyEvent *) event;
        inputs->inputsQKeyPress(ev->key());
        return true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ev = (QKeyEvent *) event;
        inputs->inputsQKeyRelease(ev->key());
        return true;
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        inputs->inputsButtonPress(SPICE_MOUSE_BUTTON_LEFT, SPICE_MOUSE_BUTTON_MASK_LEFT);
        inputs->inputsButtonRelease(SPICE_MOUSE_BUTTON_LEFT, 0);
        return true;
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent *ev = (QWheelEvent *) event;
        if (ev->delta() > 0)
        {
            inputs->inputsButtonPress(SPICE_MOUSE_BUTTON_UP, QtButtonsMaskToSpice(QApplication::mouseButtons()));
            inputs->inputsButtonRelease(SPICE_MOUSE_BUTTON_UP, QtButtonsMaskToSpice(QApplication::mouseButtons()));
        }
        else
        {
            inputs->inputsButtonPress(SPICE_MOUSE_BUTTON_DOWN, QtButtonsMaskToSpice(QApplication::mouseButtons()));
            inputs->inputsButtonRelease(SPICE_MOUSE_BUTTON_DOWN, QtButtonsMaskToSpice(QApplication::mouseButtons()));

        }
        ev->accept();
        return true;
    }


    return false;

}

void QSpiceWidget::resizeEvent ( QResizeEvent * event )
{
    Q_UNUSED(event)

    if (m_Image->updatesEnabled() && main && display)
        resizeTimer.start(500);
}

void QSpiceWidget::resizeDone()
{
    if (m_Image->updatesEnabled() && main && display)
    {
        main->mainSetDisplay(display->getId(), 0, 0, size().width(), height());
        main->mainSetDisplayEnabled(display->getId(), true);
        main->mainSendMonitorConfig();
    }

}
