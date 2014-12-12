#include "log_dock.h"

LogDock::LogDock(QWidget *parent) :
    QWidget(parent)
{
    docLayout = new QVBoxLayout(this);
    dockIcon = new QLabel(this);
    dockIcon->setPixmap(
                QIcon::fromTheme("document-properties")
                .pixmap(this->font().SizeResolved));
    currentTime = new QLabel(this);
    saveLog = new QPushButton(QIcon::fromTheme("document-save"), "", this);
    saveLog->setToolTip("Save Log to File");
    titleLayout = new QHBoxLayout();
    titleLayout->addWidget(dockIcon, 0, Qt::AlignLeft);
    titleLayout->addWidget(currentTime, 0, Qt::AlignRight);
    titleLayout->addWidget(saveLog, 0, Qt::AlignRight);
    title = new QWidget(this);
    title->setLayout(titleLayout);

    Log = new QTextBrowser(this);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2")
                    .arg(LOG_SIZE)
                    .arg(Log->toPlainText().count()));
    Log->setReadOnly(true);
    Log->setOpenLinks(false);
    Log->setOpenExternalLinks(true);
    Log->setContextMenuPolicy(Qt::DefaultContextMenu);   //Qt::CustomContextMenu);
    //connect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));
    connect(Log, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(openLink(QUrl)));

    docLayout->addWidget(title);
    docLayout->addWidget(Log);
    setLayout(docLayout);
    timerId = startTimer(1000);
    connect(saveLog, SIGNAL(clicked()),
            this, SLOT(saveLogToFile()));
}
LogDock::~LogDock()
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    };
    disconnect(saveLog, SIGNAL(clicked()),
               this, SLOT(saveLogToFile()));
    delete dockIcon;
    dockIcon = NULL;
    delete currentTime;
    currentTime = 0;
    delete saveLog;
    saveLog = NULL;
    //disconnect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));
    connect(Log, SIGNAL(anchorClicked(QUrl)),
            this, SLOT(openLink(QUrl)));
    Log->clear();
    delete Log;
    Log = NULL;
    delete docLayout;
    docLayout = NULL;
}

/* public slots */
void LogDock::appendErrorMsg(QString &msg)
{
    if ( Log->toPlainText().count()>LOG_SIZE ) Log->clear();
    Log->append(msg);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2")
                    .arg(LOG_SIZE)
                    .arg(Log->toPlainText().count()));
}

/* private slots */
void LogDock::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() )
        currentTime->setText(QString("Current Time: <b>%1</b>").arg(QTime::currentTime().toString()));
}
void LogDock::openLink(QUrl url)
{
    //qDebug()<<url.toString();
    QDesktopServices::openUrl(url);
}
void LogDock::saveLogToFile()
{
    saveLog->setEnabled(false);
    saveLog->setDown(true);
    QString _fileName = QFileDialog::getSaveFileName(
                this,
                "Save Log To",
                "~");
    if ( !_fileName.isEmpty() ) {
        QFile file;
        file.setFileName(_fileName);
        file.open(QIODevice::WriteOnly);
        QByteArray text = Log->toPlainText().toLocal8Bit();
        int written = file.write(text);
        file.close();
        if ( written==text.size() ) {
            Log->clear();
        } else {
            QString msg;
            msg.append(QString("Save Log in %1 failed.").arg(_fileName));
            appendErrorMsg(msg);
        };
    };
    saveLog->setDown(false);
    saveLog->setEnabled(true);
}
