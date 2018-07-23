#include "wait_thread.h"
#define  PERIOD 1000

Wait::Wait(QObject *parent, ConnectionList *wdgList) :
    QThread(parent), wdg(wdgList)
{

}

void Wait::run()
{
    // close connections
    while (wdg->getConnItemDataListCount()) {
        const int count = wdg->getConnItemDataListCount();
        QStringList to_Delete;
        for (int i=0; i<count; i++) {
            const ConnItemIndex *idx = wdg->getConnItemDataListIndex(i);
            if ( Q_NULLPTR==idx ) continue;
            DATA _data = idx->getData();
            ConnElement *el = static_cast<ConnElement*>(
                        wdg->getConnElementByName(idx->getName()));
            switch (_data.value("isRunning").toInt()) {
            case FAILED:
                to_Delete.append(idx->getName());
                break;
            case CLOSED:
                to_Delete.append(idx->getName());
                break;
            case RUNNING:
                if ( Q_NULLPTR!=el ) el->closeConnection();
                break;
            case CONNECT:
                if ( Q_NULLPTR!=el ) el->closeConnection();
                break;
            default:
                break;
            }
        };
        foreach (QString key, to_Delete) {
            ConnElement *el = static_cast<ConnElement*>(
                        wdg->getConnElementByName(key));
            if ( Q_NULLPTR!=el ) {
                QString _name = el->getName();
                const int count = wdg->getListItemCount();
                for (int i=0; i<count; i++) {
                    ConnItemIndex *idx = wdg->getConnItemDataListIndex(i);
                    if ( idx!=Q_NULLPTR && idx->getName().compare(_name)==0 ) {
                        const int row = wdg->getConnItemDataListIndexOf(idx);
                        wdg->removeListItem(row);
                        wdg->removeConnItemDataList(idx);
                        break;
                    };
                };
            };
            wdg->removeConnectionItembyName(key);
        };
        msleep(PERIOD);
    };
}
