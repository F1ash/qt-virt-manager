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
#include <QSettings>
#include "log_dock_menu.h"
#include <QDebug>

#define LOG_SIZE 1048576

class LogDock : public QWidget
{
    Q_OBJECT
public:
    explicit LogDock(QWidget *parent = NULL);
    ~LogDock();

private:
    bool              useNameTemplate, saveAtExit, lastProbe;
    int               timerId;
    QSettings         settings;
    QSize             _size;
    QVBoxLayout      *docLayout;
    QLabel           *currentTime;
    QPushButton      *saveLog;
    QPushButton      *menuBtn;
    LogDockMenu      *menu;
    QHBoxLayout      *titleLayout;
    QWidget          *title;
    QTextBrowser     *Log;

signals:
    void              overflow(bool);

public slots:
    void              appendMsgToLog(QString&);

private slots:
    void              timerEvent(QTimerEvent*);
    void              openLink(QUrl);
    void              _saveLogToFile();
    void              saveLogToFile();
    QString           getTemplateFilename() const;
    void              changeSettings(QAction*);

};

#endif // LOG_DOCK_H
