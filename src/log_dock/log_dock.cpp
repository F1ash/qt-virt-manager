#include "log_dock.h"

LogDock::LogDock(QWidget *parent) :
    QWidget(parent)
{
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    docLayout = new QVBoxLayout();
    currentTime = new QLabel();

    Log = new QTextEdit(this);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
    Log->setReadOnly(true);
    Log->setContextMenuPolicy(Qt::DefaultContextMenu);   //Qt::CustomContextMenu);
    //connect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));

    docLayout->addWidget(currentTime);
    docLayout->addWidget(Log);
    setLayout(docLayout);
    timerId = startTimer(1000);
}
LogDock::~LogDock()
{
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    };
    delete currentTime;
    currentTime = 0;
    disconnect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));
    Log->clear();
    delete Log;
    Log = 0;
    delete docLayout;
    docLayout = 0;
}

/* public slots */
void LogDock::appendErrorMsg(QString &msg)
{
    if ( Log->toPlainText().count()>LOG_SIZE ) Log->clear();
    Log->append(msg);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
}

/* private slots */
void LogDock::timerEvent(QTimerEvent *ev)
{
    if ( timerId==ev->timerId() )
        currentTime->setText(QString("Current Time: <b>%1</b>").arg(QTime::currentTime().toString()));
}
