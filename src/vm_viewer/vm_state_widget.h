#ifndef VM_STATE_WIDGET_H
#define VM_STATE_WIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>

#define TIMEOUT     60*1000

class VM_State_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit VM_State_Widget(QWidget *parent = NULL);
    void             setCloseProcessValue(int);

signals:
    void             showUsbDevWidget();

private:
    QLabel          *smartCard, *mouse, *keyboard,
                    *display, *webdav, *playback, *record;
    QPushButton     *usbRedir;
    QProgressBar    *closeProcess;
    QProgressBar    *downloadProcess;
    QHBoxLayout     *commoLayout;

public slots:
    void             setDownloadProcessRange(int);
    void             setDownloadProcessValue(int, int);
    void             changeSmartcardState(bool);
    void             changeMouseState(bool);
    void             changeKeyboardState(bool);
    void             changeDisplayState(bool);
    void             changeUsbredirState(bool);
    void             changeWebDAVState(bool);
    void             changePlaybackState(bool);
    void             changeRecordState(bool);
};

#endif // VM_STATE_WIDGET_H
