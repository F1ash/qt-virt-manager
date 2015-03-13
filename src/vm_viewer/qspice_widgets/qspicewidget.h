#ifndef QSPICEWIDGET_H
#define QSPICEWIDGET_H

#include <QTimer>
#include <QLabel>
#include <QScrollArea>

#include "qspicesession.h"
#include "qspicemainchannel.h"
#include "qspicedisplaychannel.h"
#include "qspiceinputschannel.h"
#include "qspicecursorchannel.h"

class QSpiceWidget : public QScrollArea
{
    Q_OBJECT
public:
    explicit QSpiceWidget(QWidget *parent = 0);
    ~QSpiceWidget();

    bool Connect(QString uri);

signals:
    void DisplayResize(const QSize &size);

protected:
    QSpiceSession           *spiceSession;
    QSpiceMainChannel       *main;
    QSpiceDisplayChannel    *display;
    QSpiceInputsChannel     *inputs;
    QSpiceCursorChannel     *cursor;

    QLabel                  *m_Image;
    QTimer                   resizeTimer;


protected slots:

    void resizeDone();

    void ChannelNew(QSpiceChannel *channel);

    void channelDestroyed();

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
    void resizeEvent ( QResizeEvent * event );
};

#endif // QSPICEWIDGET_H
