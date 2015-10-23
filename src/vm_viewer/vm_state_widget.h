#ifndef VM_STATE_WIDGET_H
#define VM_STATE_WIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QIcon>
#include <QPushButton>
#include <QHBoxLayout>
#include "transformation_mode_menu.h"

#define TIMEOUT     60*1000

class VM_State_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit VM_State_Widget(QWidget *parent = NULL);
    void             setCloseProcessValue(int);

signals:
    void             transformationMode(Qt::TransformationMode);
    void             showUsbDevWidget();

private:
    QLabel          *smartCard, *mouse, *keyboard,
                    *webdav, *playback, *record;
    QPushButton     *display, *usbRedir;
    QProgressBar    *closeProcess;
    QProgressBar    *downloadProcess;
    QHBoxLayout     *commoLayout;
    TransformationModeMenu
                    *tr_menu;

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
