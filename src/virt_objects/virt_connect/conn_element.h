#ifndef CONN_ELEMENT_H
#define CONN_ELEMENT_H

#include <QSettings>
#include <QTime>
#include <QTimerEvent>
#include "conn_alive_thread.h"
#include "settings/conn_settings.h"
#include "connection_list/conn_item_model.h"
//#include <QDebug>

#define AVAILABLE     true
#define NOT_AVAILABLE false

class ConnElement : public QObject
{
    Q_OBJECT
public:
    explicit ConnElement(QObject *parent = Q_NULLPTR);
    void              buildURI();
    bool              getOnViewState() const;
    void              overviewOfConnection();
    void              disableOverviewOfConnection();
    virConnectPtr*    getPtr_connectionPtr() const;
    void              setItemReference(ConnItemModel*, ConnItemIndex*);
    void              setItemReferenceForLocal(ConnItemModel*, ConnItemIndex*);
    void              openConnection();
    void              closeConnection();
    void              setAuthCredentials(const QString&, const QString&);
    QString           getName() const;
    QString           getURI() const;
    void              setName(const QString&);
    void              setURI(const QString&);

signals:
    void              warningShowed();
    void              warning(const QString&);
    void              connToOverview(virConnectPtr*, const QString&);
    void              authRequested(const QString&);
    void              domStateChanged();
    void              netStateChanged();
    void              poolStateChanged();
    void              secStateChanged();
    void              connClosed(bool, const QString&);
    void              domainEnd(const QString&);
    void              newOpenedConnection(const QString&);

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

    void              addMsgToLog(const QString, const QString);
    void              sendWarning(const QString&);
    void              mainWindowUp();

private slots:
    void              setConnectionState(CONN_STATE);
    void              timerEvent(QTimerEvent*);
    void              receiveConnMessage(const QString&);
    void              writeErrorToLog(const QString&, const uint);
    void              getAuthCredentials(const QString&);
    void              forwardConnClosedSignal(bool);
    void              connAliveThreadStarted();
    void              connAliveThreadFinished();
    void              emitDomainKeyToCloseViewer(const QString&);
};

#endif   // CONN_ELEMENT_H
