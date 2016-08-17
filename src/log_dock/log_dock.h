#ifndef LOG_DOCK_H
#define LOG_DOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include <QCheckBox>
#include <QSpinBox>
#include "text_browser.h"
#include <QDebug>

#define ONE_MB 1048576

class LogDock : public QWidget
{
    Q_OBJECT
public:
    explicit LogDock(QWidget *parent = nullptr);
    ~LogDock();
    TextBrowser      *Log;
    void              setUsageInSoftTouched(bool);

private:
    bool              lastProbe;
    int               timerId, currLogSize;
    QSettings         settings;
    QSize             _size;
    QVBoxLayout      *docLayout;
    QLabel           *currentTime;
    QSpinBox         *logSize;
    QCheckBox        *autoSaveLog;
    QPushButton      *saveLog;
    QHBoxLayout      *titleLayout;
    QWidget          *title;

signals:
    void              overflow(bool);

public slots:
    void              appendMsgToLog(const QString&);

private slots:
    void              timerEvent(QTimerEvent*);
    void              openLink(QUrl);
    void              _saveLogToFile();
    void              saveLogToFile();
    QString           getTemplateFilename() const;
    void              changeLogSize(int);

};

#endif // LOG_DOCK_H
