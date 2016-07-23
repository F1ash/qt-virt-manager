#ifndef VM_STATE_WIDGET_H
#define VM_STATE_WIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QIcon>
#include <QHBoxLayout>
#include "transformation_mode_menu.h"
#include "create_widgets/common/click_label.h"

#define TIMEOUT     60*1000

class VM_State_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit VM_State_Widget(QWidget *parent = nullptr);
    void             setCloseProcessValue(int);

signals:
    void             transformationMode(Qt::TransformationMode);
    void             showUsbDevWidget();
    void             showSmartCardWidget();

private:
    QLabel          *mouse, *keyboard,
                    *webdav, *playback, *record;
    Click_Label     *smartCard, *display, *usbRedir;
    QProgressBar    *closeProcess;
    QProgressBar    *downloadProcess;
    QHBoxLayout     *commoLayout;
    Qt::TransformationMode
                     tr_mode;

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

private slots:
    void             showTransformationModeMenu();
};

#endif // VM_STATE_WIDGET_H
