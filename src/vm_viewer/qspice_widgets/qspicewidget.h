#ifndef QSPICEWIDGET_H
#define QSPICEWIDGET_H

#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>

#include "qspicesession.h"
#include "qspicemainchannel.h"
#include "qspicedisplaychannel.h"
#include "qspiceinputschannel.h"
#include "qspicecursorchannel.h"
#include "qspicesmartcardchannel.h"
#include "qspiceusbredirchannel.h"
#include "qspicewebdavchannel.h"
#include "qspiceusbdevicemanager.h"

class QSpiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSpiceWidget(QWidget *parent = 0);
    ~QSpiceWidget();

    bool Connect(QString uri);
    void Disconnect();
    void SendKeySequience(Qt::Key);
    void mainFileCopyAsync(QStringList&);
    void copyClipboardFromGuest();
    void sendClipboardDataToGuest(QString&);

signals:
    void DisplayResize(const QSize &size);
    void downloaded(int, int);
    void cursorChannelChanged(bool);
    void inputsChannelChanged(bool);
    void displayChannelChanged(bool);
    void usbredirChannelChanged(bool);
    void smartcardChannelChanged(bool);
    void webdavChannelChanged(bool);

protected:
    friend class Spice_Viewer;
    QSpiceSession           *spiceSession;
    QSpiceMainChannel       *main;
    QSpiceDisplayChannel    *display;
    QSpiceInputsChannel     *inputs;
    QSpiceCursorChannel     *cursor;
    QSpiceSmartcardChannel  *smartcard;
    QSpiceUSBRedirChannel   *usbredir;
    QSpiceWebDAVChannel     *webdav;
    QSpiceUsbDeviceManager  *usbDevManager;

    QLabel                  *m_Image;
    QTimer                   resizeTimer;
    QHBoxLayout             *commonLayout;
    int                      _height, _width, WIDTH;

protected slots:
    friend class Spice_Viewer;
    void resizeDone();

    void ChannelNew(QSpiceChannel *channel);

    void channelDestroyed();

    void mainAgentUpdate();
    void mainClipboardSelection(QString&);
    void mainClipboardSelectionGrab();
    void mainClipboardSelectionRelease(uint);
    void mainClipboardSelectionRequest(uint, uint);
    void mainMouseUpdate();

    void displayPrimaryCreate(
         int                 format,
         int                 width,
         int                 height,
         int                 stride,
         int                 shmid,
         void *              imgdata);

    void displayInvalidate(
        int                 x,
        int                 y,
        int                 width,
        int                 height);

    void displayPrimaryDestroy();

    void displayMark(int mark);

    void cursorSet(
            int                width,
            int                height,
            int                hot_x,
            int                hot_y,
            void *             rgba);


    bool eventFilter(QObject *object, QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void setDifferentSize(int, int, int);
};

#endif // QSPICEWIDGET_H
