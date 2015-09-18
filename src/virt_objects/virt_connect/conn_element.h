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
    explicit ConnElement(QObject *parent = NULL);
    void              buildURI();

signals:
    void              warningShowed();
    void              warning(QString&);
    void              connPtrPtr(virConnectPtr*, QString&);
    void              authRequested(QString&);
    void              domStateChanged(Result);
    void              netStateChanged(Result);
    void              connClosed(bool, QString&);
    void              domainEnd(QString&);

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

    ConnAliveThread  *connAliveThread;

public slots:
    void              setItemReference(ConnItemModel*, ConnItemIndex*);
    void              setItemReferenceForLocal(ConnItemModel*, ConnItemIndex*);
    void              openConnection();
    void              closeConnection();
    void              overviewConnection();
    virConnectPtr*    getPtr_connectionPtr() const;
    void              setAuthCredentials(QString&, QString&);
    QString           getName() const;
    QString           getURI() const;
    void              setName(QString&);
    void              setURI(QString&);
    void              setOnViewConnAliveThread(bool);

private slots:
    void              setConnectionState(CONN_STATE);
    void              timerEvent(QTimerEvent*);
    void              receiveConnMessage(QString);
    void              addMsgToLog(QString, QString);
    void              sendWarning(QString&);
    void              writeErrorToLog(QString&, uint);
    void              mainWindowUp();
    void              getAuthCredentials(QString&);
    void              forwardConnClosedSignal(bool);
    void              connAliveThreadStarted();
    void              connAliveThreadFinished();
    void              emitDomainKeyToCloseViewer(QString&);
};

#endif   // CONN_ELEMENT_H
