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
#include "spiceusbdevicewidget.h"
#include "qspicesmartcardmanager.h"

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

    void errMsg(QString&);

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
    QSpiceSmartcardManager  *smartcardManager;

    QLabel                  *m_Image;
    QTimer                   resizeTimer;
    QHBoxLayout             *commonLayout;
    int                      _height, _width, WIDTH;

private slots:
    void ChannelNew(QSpiceChannel *channel);
    void channelDestroyed();

    void mainAgentUpdate();
    void mainClipboardSelection(QString&);
    void mainClipboardSelectionGrab();
    void mainClipboardSelectionRelease(uint);
    void mainClipboardSelectionRequest(uint, uint);
    void mainMouseUpdate();

    void usbDevAutoConnectFailed(QString&, QString&);
    void usbDevAdded(QString&);
    void usbDevError(QString&, QString&);
    void usbDevRemoved(QString&);

    void cardInserted(QString&);
    void cardRemoved(QString&);
    void readerAdded(QString&);
    void readerRemoved(QString&);

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
    void reloadUsbDevList(void*);

protected slots:
    void resizeDone();
    void setDifferentSize(int, int, int);
    void showUsbDevWidget();
};

#endif // QSPICEWIDGET_H
