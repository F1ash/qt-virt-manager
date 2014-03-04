#include "log_dock.h"

LogDock::LogDock(QWidget *parent) :
    QWidget(parent)
{
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    docLayout = new QVBoxLayout();

    Log = new QTextEdit(this);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
    Log->setReadOnly(true);
    Log->setContextMenuPolicy(Qt::DefaultContextMenu);   //Qt::CustomContextMenu);
    //connect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));

    docLayout->addWidget(Log);
    setLayout(docLayout);
}
LogDock::~LogDock()
{
    disconnect(Log, SIGNAL(customContextMenuRequested(const QPoint&)), Log, SLOT(clear()));
    Log->clear();
    delete Log;
    Log = 0;
    delete docLayout;
    docLayout = 0;
}
void LogDock::appendErrorMsg(QString &msg)
{
    if ( Log->toPlainText().count()>LOG_SIZE ) Log->clear();
    Log->append(msg);
    Log->setToolTip(QString("Event/Error Log\nMaxSize:\t%1 Bytes\nCurrent:\t%2").arg(LOG_SIZE).arg(Log->toPlainText().count()));
}
