#ifndef CONN_ELEMENT_H
#define CONN_ELEMENT_H

#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include "conn_alive_thread.h"
#include "settings/conn_settings.h"
#include "layout/conn_item_model.h"
#include <QDebug>

#define TIMEOUT 10
#define TO_RUN true
#define TO_STOP false
#define AVAILABLE true
#define NOT_AVAILABLE false

class ElemConnect : public QObject
{
    Q_OBJECT

public:
    ElemConnect(QObject *parent);
    ~ElemConnect();

public slots:
    void setItemReference(ConnItemModel*, ConnItemIndex*);
    void openConnect();
    void showConnectData();
    void closeConnect();

signals:
    void warningShowed();
    void warning(QString&);
    void connPtr(virConnect*, QString&);

private:
    ConnItemModel    *own_model;
    ConnItemIndex    *own_index;

    QString           name;
    QString           Host;
    QString           URI;
    DATA              conn_Status;
    QSettings         settings;
    int               waitTimerId;

    int               checkTimeout;
    int               _diff;

    ConnAliveThread  *connAliveThread = NULL;

private slots:
    void buildCommand();
    void setConnectState(CONN_STATE);
    void timerEvent(QTimerEvent*);
    void receiveConnMessage(QString);
    void addMsgToLog(QString, QString);
    void sendWarning(QString&);
    void mainWindowUp();
};

#endif   // CONN_ELEMENT_H
