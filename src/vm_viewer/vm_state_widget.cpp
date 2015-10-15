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
    display = new QLabel(this);
    display->setContentsMargins(0,0,0,0);
    display->setPixmap(QIcon::fromTheme("video-display")
                       .pixmap(fontInfo().pixelSize(), QIcon::Disabled));
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
    setLayout(commoLayout);
    closeProcess->hide();
    downloadProcess->setToolTip("File download progress");
    smartCard->setToolTip("SmartCard channel");
    mouse->setToolTip("Cursor channel");
    keyboard->setToolTip("Inputs channel");
    display->setToolTip("Display channel");
    usbRedir->setToolTip("USB Redir channel\nClick to choose devices");
    webdav->setToolTip("WebDAV channel");
    playback->setToolTip("Playback channel");
    setContentsMargins(0,0,0,0);
    connect(usbRedir, SIGNAL(released()),
            this, SIGNAL(showUsbDevWidget()));
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
    int value = downloadProcess->value()+_val1;
    downloadProcess->setValue(value);
}
void VM_State_Widget::changeSmartcardState(bool state)
{
    smartCard->setPixmap(
                QIcon::fromTheme("media-flash")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
void VM_State_Widget::changeMouseState(bool state)
{
    mouse->setPixmap(
                QIcon::fromTheme("input-mouse")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
void VM_State_Widget::changeKeyboardState(bool state)
{
    keyboard->setPixmap(
                QIcon::fromTheme("input-keyboard")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
void VM_State_Widget::changeDisplayState(bool state)
{
    display->setPixmap(
                QIcon::fromTheme("video-display")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
void VM_State_Widget::changeUsbredirState(bool state)
{
    usbRedir->setEnabled(state);
}
void VM_State_Widget::changeWebDAVState(bool state)
{
    webdav->setPixmap(
                QIcon::fromTheme("folder-remote")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
void VM_State_Widget::changePlaybackState(bool state)
{
    playback->setPixmap(
                QIcon::fromTheme("audio-card")
                .pixmap(fontInfo().pixelSize(), (state)? QIcon::Active : QIcon::Disabled));
}
