#include "vm_state_widget.h"

VM_State_Widget::VM_State_Widget(QWidget *parent) : QWidget(parent)
{
    smartCard = new QLabel(this);
    smartCard->setContentsMargins(0,0,0,0);
    smartCard->setPixmap(QIcon::fromTheme("media-flash")
                         .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    mouse = new QLabel(this);
    mouse->setContentsMargins(0,0,0,0);
    mouse->setPixmap(QIcon::fromTheme("input-mouse")
                     .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    keyboard = new QLabel(this);
    keyboard->setContentsMargins(0,0,0,0);
    keyboard->setPixmap(QIcon::fromTheme("input-keyboard")
                        .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    tr_menu = new TransformationModeMenu(this);
    display = new QPushButton(QIcon::fromTheme("video-display"), "", this);
    display->setContentsMargins(0,0,0,0);
    display->setEnabled(false);
    display->setMenu(tr_menu);
    display->setFlat(true);
    usbRedir = new QPushButton(QIcon::fromTheme("drive-removable-media"), "", this);
    usbRedir->setContentsMargins(0,0,0,0);
    usbRedir->setEnabled(false);
    usbRedir->setFlat(true);
    webdav = new QLabel(this);
    webdav->setContentsMargins(0,0,0,0);
    webdav->setPixmap(QIcon::fromTheme("folder-remote")
                         .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    playback = new QLabel(this);
    playback->setContentsMargins(0,0,0,0);
    playback->setPixmap(QIcon::fromTheme("audio-card")
                       .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    record = new QLabel(this);
    record->setContentsMargins(0,0,0,0);
    record->setPixmap(QIcon::fromTheme("audio-card")
                      .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
    closeProcess = new QProgressBar(this);
    closeProcess->setContentsMargins(0,0,0,0);
    closeProcess->setRange(0, TIMEOUT);
    downloadProcess = new QProgressBar(this);
    downloadProcess->setContentsMargins(0,0,0,0);
    commoLayout = new QHBoxLayout(this);
    commoLayout->setContentsMargins(0,0,0,0);
    commoLayout->addWidget(closeProcess);
    commoLayout->addWidget(downloadProcess);
    commoLayout->addWidget(smartCard);
    commoLayout->addWidget(mouse);
    commoLayout->addWidget(keyboard);
    commoLayout->addWidget(display);
    commoLayout->addWidget(usbRedir);
    commoLayout->addWidget(webdav);
    commoLayout->addWidget(playback);
    commoLayout->addWidget(record);
    setLayout(commoLayout);
    closeProcess->hide();
    downloadProcess->setToolTip("File download progress");
    smartCard->setObjectName("SmartCard channel");
    mouse->setObjectName("Cursor channel");
    keyboard->setObjectName("Inputs channel");
    display->setObjectName("Display channel");
    usbRedir->setObjectName("USB Redir channel");
    webdav->setObjectName("WebDAV channel");
    playback->setObjectName("Playback channel");
    record->setObjectName("Record channel");
    setContentsMargins(0,0,0,0);
    connect(usbRedir, SIGNAL(released()),
            this, SIGNAL(showUsbDevWidget()));
    connect(tr_menu, SIGNAL(new_mode(Qt::TransformationMode)),
            this, SIGNAL(transformationMode(Qt::TransformationMode)));
}

/* public slots */
void VM_State_Widget::setCloseProcessValue(int _value)
{
    closeProcess->setValue(_value);
}
void VM_State_Widget::setDownloadProcessRange(int _value)
{
    downloadProcess->setRange(0, _value);
}
void VM_State_Widget::setDownloadProcessValue(int _val1, int _val2)
{
    downloadProcess->setRange(0, _val2);
    downloadProcess->setValue(_val1);
}
void VM_State_Widget::changeSmartcardState(bool state)
{
    smartCard->setPixmap(
                QIcon::fromTheme("media-flash")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    smartCard->setToolTip(
                QString("%1 (%2)")
                .arg(smartCard->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeMouseState(bool state)
{
    mouse->setPixmap(
                QIcon::fromTheme("input-mouse")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    mouse->setToolTip(
                QString("%1 (%2)")
                .arg(mouse->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeKeyboardState(bool state)
{
    keyboard->setPixmap(
                QIcon::fromTheme("input-keyboard")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    keyboard->setToolTip(
                QString("%1 (%2)")
                .arg(keyboard->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeDisplayState(bool state)
{
    display->setEnabled(state);
    display->setToolTip(
                QString("%1 (%2)")
                .arg(display->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeUsbredirState(bool state)
{
    usbRedir->setEnabled(state);
    usbRedir->setToolTip(
                QString("%1 (%2)\nClick to choose devices")
                .arg(usbRedir->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeWebDAVState(bool state)
{
    webdav->setPixmap(
                QIcon::fromTheme("folder-remote")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    webdav->setToolTip(
                QString("%1 (%2)")
                .arg(webdav->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changePlaybackState(bool state)
{
    playback->setPixmap(
                QIcon::fromTheme("audio-card")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    playback->setToolTip(
                QString("%1 (%2)")
                .arg(playback->objectName())
                .arg(state? "ON":"OFF"));
}
void VM_State_Widget::changeRecordState(bool state)
{
    record->setPixmap(
                QIcon::fromTheme("audio-card")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
    record->setToolTip(
                QString("%1 (%2)")
                .arg(record->objectName())
                .arg(state? "ON":"OFF"));
}
