#include "wait_thread.h"
#define  PERIOD 333

Wait::Wait(QObject *parent, ConnectionList *wdgList) :
    QThread(parent), wdg(wdgList)
{

}

void Wait::run()
{
    // close connections
    while (wdg->connItemModel->connItemDataList.count()) {
        int count = wdg->connItemModel->connItemDataList.count();
        QStringList to_Delete;
        for (int i=0; i<count; i++) {
            ConnItemIndex *idx = wdg->connItemModel->connItemDataList.at(i);
            if ( NULL==idx ) continue;
            DATA _data = idx->getData();
            if ( _data.value("isRunning").toInt()!=RUNNING ) {
                to_Delete.append(idx->getName());
            } else {
                ConnElement *el = static_cast<ConnElement*>(
                            wdg->connections->value(idx->getName()));
                if ( NULL!=el ) el->closeConnection();
            };
        };
        foreach (QString key, to_Delete) {
            ConnElement *el = static_cast<ConnElement*>(
                        wdg->connections->value(key));
            if ( NULL!=el ) {
                QString _name = el->getName();
                int count = wdg->connItemModel->rowCount();
                ConnItemIndex *idx = NULL;
                for (int i=0; i<count; i++) {
                    idx = wdg->connItemModel->connItemDataList.at(i);
                    if ( idx->getName()==_name ) {
                        int row = wdg->connItemModel->connItemDataList.indexOf(idx);
                        wdg->connItemModel->removeRow(row);
                        wdg->connItemModel->connItemDataList.removeOne(idx);
                        break;
                    };
                };
                delete el;
                el = NULL;
                wdg->connections->remove(key);
            };
        };
        msleep(PERIOD);
    };
}
