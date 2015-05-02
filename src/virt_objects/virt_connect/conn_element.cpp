#include "conn_element.h"

#define TIMEOUT 10
#define TO_RUN true
#define TO_STOP false

ElemConnect::ElemConnect(QObject *parent) :
    QObject(parent)
{
    //connect(this, SIGNAL(readyRead()), this, SLOT(sendMessage()));
    waitTimerId = 0;

    connAliveThread = new ConnAliveThread(this);
    connect(connAliveThread, SIGNAL(connMsg(QString)),
            this, SLOT(receiveConnMessage(QString)));
    connect(connAliveThread, SIGNAL(changeConnState(CONN_STATE)),
            this, SLOT(setConnectState(CONN_STATE)));
    connect(connAliveThread, SIGNAL(authRequested(QString&)),
            this, SLOT(getAuthCredentials(QString&)));
    connect(connAliveThread, SIGNAL(domStateChanged(Result)),
            this, SIGNAL(domStateChanged(Result)));
}
ElemConnect::~ElemConnect()
{
    //disconnect(this, SIGNAL(readyRead()), this, SLOT(sendMessage()));
    disconnect(connAliveThread, SIGNAL(connMsg(QString)),
               this, SLOT(receiveConnMessage(QString)));
    disconnect(connAliveThread, SIGNAL(changeConnState(CONN_STATE)),
               this, SLOT(setConnectState(CONN_STATE)));
    disconnect(connAliveThread, SIGNAL(authRequested(QString&)),
               this, SLOT(getAuthCredentials(QString&)));
    disconnect(connAliveThread, SIGNAL(domStateChanged(Result)),
               this, SIGNAL(domStateChanged(Result)));

    if ( connAliveThread!=NULL ) {
        connAliveThread->setKeepAlive(false);
        connAliveThread->quit();
        delete connAliveThread;
        connAliveThread = NULL;
    };
}

/* public slots */
void ElemConnect::setItemReference(ConnItemModel *model, ConnItemIndex *idx)
{
    own_model = model;
    own_index = idx;
    name = idx->getName();
    conn_Status.insert("availability", QVariant(AVAILABLE));
    conn_Status.insert("isRunning", QVariant(STOPPED));
    conn_Status.insert("initName", QVariant(name));
    idx->setData(conn_Status);
    settings.beginGroup("Connects");
    settings.beginGroup(name);
    bool atStart = settings.value("AtStart", QVariant(false)).toBool();
    settings.endGroup();
    settings.endGroup();
    if ( atStart ) openConnect();
}
void ElemConnect::openConnect()
{
    conn_Status.insert("availability", QVariant(NOT_AVAILABLE));
    conn_Status.insert("isRunning", QVariant(RUNNING));
    own_index->setData(conn_Status);
    buildCommand();
    connAliveThread->setData(URI);
    connAliveThread->start();
    if (!waitTimerId) {
        _diff = 0;
        waitTimerId = startTimer(1000);
    } else {
        addMsgToLog(QString("Connection '%1'").arg(name),
            QString("Wait Timer is running: %1").arg(waitTimerId));
    };
}
void ElemConnect::closeConnect()
{
    connAliveThread->setKeepAlive(false);
}
void ElemConnect::showConnectData()
{
    virConnect *conn = NULL;
    conn = connAliveThread->getConnect();
    //qDebug()<<"showConnectData:"<<name<<QVariant((conn!=NULL)?true:false).toString()<<conn;
    emit connPtr(conn, name);
    int row = own_model->connItemDataList.indexOf(own_index);
    own_model->setData(own_model->index(row, 0), true, Qt::DecorationRole);
}
virConnect* ElemConnect::getConnect() const
{
    return connAliveThread->getConnect();
}
void ElemConnect::setAuthCredentials(QString &crd, QString &text)
{
    if ( connAliveThread!=NULL ) {
        connAliveThread->setAuthCredentials(crd, text);
    }
}
QString ElemConnect::getName() const
{
    return name;
}
void ElemConnect::setOnViewConnAliveThread(bool state)
{
    connAliveThread->onView = state;
}

/* private slots */
void ElemConnect::buildCommand()
{
    settings.beginGroup("Connects");
    settings.beginGroup(name);
    checkTimeout = settings.value("TimeOut", TIMEOUT).toInt();
    QString Driver = settings.value("Driver", "").toString();
    QString Transport = settings.value("Transport", "").toString();
    Host = settings.value("Host", "").toString();
    QString Path = settings.value("Path", "").toString();
    QString Extra = settings.value("Extra", "").toString();
    settings.endGroup();
    settings.endGroup();

    // URI building
    QStringList _uri;
    _uri.append(Driver.toLower().toLower().split("/").first());
    if ( !Transport.isEmpty() ) {
        _uri.append("+");
        _uri.append(Transport.toLower());
    };
    _uri.append("://");
    if ( !Host.isEmpty() ) {
        _uri.append(Host);
    };
    _uri.append("/");
    if ( !Path.isEmpty() ) {
        _uri.append(Path);
    };
    if ( !Extra.isEmpty() ) {
        _uri.append("?");
        _uri.append(Extra);
    };
    URI = _uri.join("");
}
void ElemConnect::setConnectState(CONN_STATE status)
{
  if ( status!=RUNNING ) {
      if (waitTimerId) {
          killTimer(waitTimerId);
          waitTimerId = 0;
      };
      URI.clear();
      URI.append("-");
  } else _diff = checkTimeout + 1;
  conn_Status.insert("isRunning", QVariant(status));
  conn_Status.insert("availability", QVariant(AVAILABLE));
  conn_Status.insert("onView", QVariant(false));
  own_index->setData(conn_Status);
  int row = own_model->connItemDataList.indexOf(own_index);
  QString data;
  for (int i=0; i<own_model->columnCount(); i++) {
      switch (i) {
      case 0:
          data = name;
          break;
      case 1:
          data = URI;
          break;
      case 2:
          switch (status) {
          case FAILED:
              data = "FAILED";
              break;
          case RUNNING:
              data = "OPENED";
              break;
          case STOPPED:
              data = "CLOSED";
              break;
          default:
              break;
          };
          break;
      default:
          break;
      };
      own_model->setData(own_model->index(row, i), data, Qt::EditRole);
  };
}
void ElemConnect::timerEvent(QTimerEvent *event)
{
    int percent = 0;
    int _timerId = event->timerId();
    if ( _timerId && waitTimerId==_timerId ) {
        if ( checkTimeout - _diff + 1 ) {
            percent = int ((float(_diff)/checkTimeout)*100.0);
            QModelIndex _idx = own_model->index( own_model->connItemDataList.indexOf( own_index ), 2 );
            own_model->setData(_idx, QString::number(percent), Qt::EditRole);
            _diff++;
        };
    };
}
void ElemConnect::receiveConnMessage(QString msg)
{
    addMsgToLog( QString("Connection '%1'").arg(name), msg );
}
void ElemConnect::addMsgToLog(QString title, QString msg)
{
    QString time = QTime::currentTime().toString();
    QString errorMsg = QString("<b>%1 %2:</b><br><font color='blue'><b>EVENT</b></font>: %3")
            .arg(time).arg(title).arg(msg);
    sendWarning(errorMsg);
    mainWindowUp();
}
void ElemConnect::sendWarning(QString &msg)
{
    emit warning(msg);
}
void ElemConnect::mainWindowUp()
{
    emit warningShowed();
}
void ElemConnect::getAuthCredentials(QString &crd)
{
    emit authRequested(crd);
}
