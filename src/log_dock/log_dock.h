#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QTime>
#include <QTimerEvent>
#include <QDebug>

#define LOG_SIZE 1048576

class LogDock : public QWidget
{
    Q_OBJECT
public:
    explicit LogDock(QWidget *parent = 0);
    ~LogDock();

private:
    int           timerId;
    QSize         _size;
    QVBoxLayout  *docLayout;
    QLabel       *currentTime;
    QTextEdit    *Log;

signals:

public slots:
    void appendErrorMsg(QString&);

private slots:
    void timerEvent(QTimerEvent*);

};

#endif // LOG_DOCK_H
