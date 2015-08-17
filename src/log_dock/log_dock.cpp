#include "log_dock.h"

LogDock::LogDock(QWidget *parent) :
    QWidget(parent)
{
    settings.beginGroup("LogDock");
    useNameTemplate = settings.value("UseNameTemplate", false).toBool();
    saveAtExit = settings.value("SaveAtExit", false).toBool();
    settings.endGroup();
    // lastProbe variable for reject loop when log overflow is occurred
    // see for LogDock::saveLogToFile()
    lastProbe = true;
    setSizePolicy(
                QSizePolicy(
                    QSizePolicy::MinimumExpanding,
                    QSizePolicy::MinimumExpanding));
    int _size = this->fontInfo().pixelSize();
    currentTime = new QLabel(this);
    saveLog = new QPushButton(QIcon::fromTheme("document-save"), "", this);
    saveLog->setToolTip("Save Log to File");
    saveLog->setMaximumSize(QSize(_size, _size));
    menu = new LogDockMenu(this);
    menu->useNameTemplate->setChecked(useNameTemplate);
    menu->saveAtExit->setChecked(saveAtExit);
    menuBtn = new QPushButton(QIcon::fromTheme("system-settings"), "", this);
    menuBtn->setToolTip("Log Settings");
    menuBtn->setMaximumSize(QSize(_size, _size));
    menuBtn->setMenu(menu);
    titleLayout = new QHBoxLayout();
    titleLayout->addWidget(currentTime, 0, Qt::AlignRight);
    titleLayout->addStretch(-1);
    titleLayout->addWidget(saveLog, 0, Qt::AlignRight);
    titleLayout->addWidget(menuBtn, 0, Qt::AlignRight);
    title = new QWidget(this);
    title->setLayout(titleLayout);
    titleLayout->setMargin(0);
    title->setContentsMargins(0, 0, 10, 0);

    Log = new QTextBrowser(this);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2")
                    .arg(LOG_SIZE)
                    .arg(Log->toPlainText().count()));
    Log->setReadOnly(true);
    Log->setOpenLinks(false);
    Log->setOpenExternalLinks(true);
    Log->setContextMenuPolicy(Qt::DefaultContextMenu);   //Qt::CustomContextMenu);
    connect(Log, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(openLink(QUrl)));

    docLayout = new QVBoxLayout(this);
    docLayout->addWidget(title);
    docLayout->addWidget(Log);
    setLayout(docLayout);
    docLayout->setMargin(0);
    setContentsMargins(0, 0, 0, 0);
    timerId = startTimer(1000);
    connect(saveLog, SIGNAL(clicked()),
            this, SLOT(_saveLogToFile()));
    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(changeSettings(QAction*)));
}
LogDock::~LogDock()
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    };
    settings.beginGroup("LogDock");
    settings.setValue("UseNameTemplate", useNameTemplate);
    settings.setValue("SaveAtExit", saveAtExit);
    settings.endGroup();
    settings.sync();
    useNameTemplate = true;
    if ( saveAtExit ) saveLogToFile();
}

/* public slots */
void LogDock::appendMsgToLog(QString &msg)
{
    Log->append(msg);
    Log->setToolTip(
                QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2")
                .arg(LOG_SIZE)
                .arg(Log->toPlainText().count()));
    if ( Log->toPlainText().count()>LOG_SIZE ) {
        if ( useNameTemplate ) {
            saveLogToFile();
        } else {
            emit overflow(true);
        };
    };
}

/* private slots */
void LogDock::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() )
        currentTime->setText(
                    QString("Current Time: <b>%1</b>")
                    .arg(QTime::currentTime().toString()));
}
void LogDock::openLink(QUrl url)
{
    //qDebug()<<url.toString();
    QDesktopServices::openUrl(url);
}
void LogDock::_saveLogToFile()
{
    lastProbe = true;
    saveLogToFile();
}
void LogDock::saveLogToFile()
{
    if ( !lastProbe ) return;
    saveLog->setEnabled(false);
    saveLog->setDown(true);
    QString _fileName;
    if ( useNameTemplate ) {
        _fileName = getTemplateFilename();
    } else {
        _fileName = QFileDialog::getSaveFileName(
                this,
                "Save Log To",
                QString("~/%1")
                    .arg(getTemplateFilename()));
    };
    if ( !_fileName.isEmpty() ) {
        QFile file;
        file.setFileName(_fileName);
        file.open(QIODevice::WriteOnly);
        QByteArray text = Log->toPlainText().toLocal8Bit();
        int written = file.write(text.data());
        file.close();
        QString msg;
        if ( written==text.size() ) {
            Log->clear();
            emit overflow(false);
            msg.append(QString("Save Log to %1 done.").arg(_fileName));
            lastProbe = true;
        } else {
            msg.append(QString("Save Log to %1 failed.").arg(_fileName));
            lastProbe = false;
        };
        QString time = QTime::currentTime().toString();
        QString currMsg = QString("<b>%1:</b><br><font color='green'><b>ACTION</b></font>: %3")
                .arg(time).arg(msg);
        appendMsgToLog(currMsg);
    };
    saveLog->setDown(false);
    saveLog->setEnabled(true);
}
QString LogDock::getTemplateFilename() const
{
    return QString("QtVirtManager_%1_%2.log")
            .arg(QDate::currentDate().toString("dd.MM.yyyy"))
            .arg(QTime::currentTime().toString());
}
void LogDock::changeSettings(QAction *act)
{
    if        ( act==menu->useNameTemplate ) {
        useNameTemplate = menu->useNameTemplate->isChecked();
    } else if ( act==menu->saveAtExit ) {
        saveAtExit = menu->saveAtExit->isChecked();
    };
}
