#ifndef QSPICEWIDGET_H
#define QSPICEWIDGET_H

#include <QTime>
#include <QDate>
#include <QTimer>
#include <QLabel>
#include <QFileDialog>
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
#include "qspiceplaybackchannel.h"
#include "qspicerecordchannel.h"

class QSpiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QSpiceWidget(QWidget *parent = 0);

    bool Connect(QString uri);
    void Disconnect();
    void SendKeySequience(Qt::Key);
    void mainFileCopyAsync(QStringList&);
    void copyClipboardFromGuest();
    void sendClipboardDataToGuest(QString&);

signals:
    void DisplayResize(const QSize&);
    void downloaded(int, int);
    void cursorChannelChanged(bool);
    void inputsChannelChanged(bool);
    void displayChannelChanged(bool);
    void usbredirChannelChanged(bool);
    void smartcardChannelChanged(bool);
    void webdavChannelChanged(bool);
    void playbackChannelChanged(bool);
    void recordChannelChanged(bool);

    void errMsg(QString&);

private:
    QString                  guestName;

protected:
    QSpiceSession           *spiceSession;
    QSpiceMainChannel       *main;
    QSpiceDisplayChannel    *display;
    QSpiceInputsChannel     *inputs;
    QSpiceCursorChannel     *cursor;
    QSpiceSmartcardChannel  *smartcard;
    QSpiceUSBRedirChannel   *usbredir;
    QSpiceWebDAVChannel     *webdav;
    QSpicePlaybackChannel   *playback;
    QSpiceRecordChannel     *record;
    QSpiceUsbDeviceManager  *usbDevManager;
    QSpiceSmartcardManager  *smartcardManager;

    Qt::TransformationMode   tr_mode;
    QImage                  *img;
    QLabel                  *m_Image;
    QTimer                   resizeTimer;
    QHBoxLayout             *commonLayout;
    int                      _height, _width,
                             init_h, init_w;
    qreal                    zoom;

private slots:
    void ChannelNew(QSpiceChannel *channel);
    void channelDestroyed();

    void mainAgentUpdate();
    void mainClipboardSelection(uint, void*, uint);
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

    void playbackData(void*, int);
    void playbackGetDelay();
    void playbackStart(int, int, int);
    void playbackStop();

    void recordStart(int, int, int);
    void recordStop();

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
    void resizeDone();

public slots:
    void setGuestName(QString&);
    void setNewSize(int, int);
    void showUsbDevWidget();
    void getScreenshot();
    void setTransformationMode(Qt::TransformationMode);
};

#endif // QSPICEWIDGET_H
