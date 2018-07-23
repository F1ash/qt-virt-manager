#include "wait_local_conn.h"

WaitLocalConn::WaitLocalConn(
        QObject        *parent,
        CONN_LIST      *_conns,
        ConnItemModel  *_model) :
    QThread(parent),
    connections(_conns),
    connItemModel(_model)
{

}

void WaitLocalConn::run()
{
    localConnsClosed = false;
    if ( connections==Q_NULLPTR || connItemModel==Q_NULLPTR ) return;
    while (!localConnsClosed) {
        QStringList to_Delete;
        bool exist = false;
        foreach (QString key, connections->keys()) {
            ConnElement *el = static_cast<ConnElement*>(
                        connections->value(key));
            if ( Q_NULLPTR!=el ) {
                QRegExp rx("^\\{Local([0-9]+)_([A-Z]+)\\}$");
                QString _name = el->getName();
                if ( _name.contains(rx) ) {
                    exist = true;
                    const int count = connItemModel->rowCount();
                    for (int i=0; i<count; i++) {
                        ConnItemIndex *idx = Q_NULLPTR;
                        if ( 0<=i && i< connItemModel->connItemDataList.count() ) {
                            idx = connItemModel->connItemDataList.at(i);
                        };
                        if ( Q_NULLPTR==idx ) continue;
                        if ( idx->getName().compare(_name)==0 ) {
                            DATA _data = idx->getData();
                            if ( _data.value("isRunning").toInt()!=RUNNING ) {
                                to_Delete.append(key);
                            } else {
                                el->closeConnection();
                            };
                        };
                    };
                } else {
                    QString uri(el->getURI());
                    emit uriToCompare(uri);
                };
            };
        };
        localConnsClosed = !exist;
        foreach (QString key, to_Delete) {
            ConnElement *el = static_cast<ConnElement*>(
                        connections->value(key));
            if ( Q_NULLPTR!=el ) {
                QString _name = el->getName();
                QList<ConnItemIndex*>::const_iterator i;
                for (i=connItemModel->connItemDataList.constBegin();
                     i!=connItemModel->connItemDataList.constEnd();
                     ++i) {
                    ConnItemIndex *idx = (*i);
                    if ( Q_NULLPTR==idx ) continue;
                    if ( idx->getName().compare(_name)==0 ) {
                        int row =
                                connItemModel->connItemDataList.indexOf(idx);
                        connItemModel->removeRow(row);
                        connItemModel->connItemDataList.removeOne(idx);
                        break;
                    };
                };
                //el->deleteLater();
            };
            connections->remove(key);
        };
        //msleep(333);
    };
}
