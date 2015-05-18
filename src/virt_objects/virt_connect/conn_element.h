#ifndef CONN_ELEMENT_H
#define CONN_ELEMENT_H

#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include "conn_alive_thread.h"
#include "settings/conn_settings.h"
#include "layout/conn_item_model.h"
#include <QDebug>

#define AVAILABLE     true
#define NOT_AVAILABLE false

class ConnElement : public QObject
{
    Q_OBJECT

public:
    ConnElement(QObject *parent);
    ~ConnElement();

public slots:
    void              setItemReference(ConnItemModel*, ConnItemIndex*);
    void              openConnection();
    void              closeConnection();
    void              showConnectionData();
    virConnect*       getConnection() const;
    void              setAuthCredentials(QString&, QString&);
    QString           getName() const;
    QString           getURI() const;
    void              setOnViewConnAliveThread(bool);

signals:
    void              warningShowed();
    void              warning(QString&);
    void              connPtr(virConnect*, QString&);
    void              authRequested(QString&);
    void              domStateChanged(Result);

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
    void              buildCommand();
    void              setConnectionState(CONN_STATE);
    void              timerEvent(QTimerEvent*);
    void              receiveConnMessage(QString);
    void              addMsgToLog(QString, QString);
    void              sendWarning(QString&);
    void              mainWindowUp();
    void              getAuthCredentials(QString&);
};

#endif   // CONN_ELEMENT_H
