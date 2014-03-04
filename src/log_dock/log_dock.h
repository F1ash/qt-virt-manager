#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QDebug>

#define LOG_SIZE 1048576

class LogDock : public QWidget
{
    Q_OBJECT
public:
    explicit LogDock(QWidget *parent = 0);
    ~LogDock();

private:
    QSize         _size;
    QVBoxLayout  *docLayout;
    QTextEdit    *Log;

signals:

public slots:
    void appendErrorMsg(QString&);

private slots:

};

#endif // LOG_DOCK_H
