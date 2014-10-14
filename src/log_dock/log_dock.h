#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QTime>
#include <QTimerEvent>
#include <QUrl>
#include <QDesktopServices>
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
    QLabel           *currentTime;
    QTextBrowser     *Log;

signals:

public slots:
    void appendErrorMsg(QString&);

private slots:
    void timerEvent(QTimerEvent*);
    void openLink(QUrl);

};

#endif // LOG_DOCK_H
