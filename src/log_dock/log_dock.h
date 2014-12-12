#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QTextBrowser>
#include <QTime>
#include <QTimerEvent>
#include <QUrl>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>
#include <QPushButton>
#include <QDebug>

#define LOG_SIZE 1048576

class LogDock : public QWidget
{
    Q_OBJECT
public:
    explicit LogDock(QWidget *parent = NULL);
    ~LogDock();

private:
    int               timerId;
    QSize             _size;
    QVBoxLayout      *docLayout;
    QLabel           *currentTime, *dockIcon;
    QPushButton      *saveLog;
    QHBoxLayout      *titleLayout;
    QWidget          *title;
    QTextBrowser     *Log;

signals:

public slots:
    void              appendErrorMsg(QString&);

private slots:
    void              timerEvent(QTimerEvent*);
    void              openLink(QUrl);
    void              saveLogToFile();

};

#endif // LOG_DOCK_H
