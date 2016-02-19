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
            if ( nullptr==idx ) continue;
            DATA _data = idx->getData();
            ConnElement *el = static_cast<ConnElement*>(
                        wdg->connections->value(idx->getName()));
            switch (_data.value("isRunning").toInt()) {
            case FAILED:
                to_Delete.append(idx->getName());
                break;
            case CLOSED:
                to_Delete.append(idx->getName());
                break;
            case RUNNING:
                if ( nullptr!=el ) el->closeConnection();
                break;
            case CONNECT:
                // undefined, still waiting to close the opening of connection
                break;
            default:
                break;
            }
        };
        foreach (QString key, to_Delete) {
            ConnElement *el = static_cast<ConnElement*>(
                        wdg->connections->value(key));
            if ( nullptr!=el ) {
                QString _name = el->getName();
                int count = wdg->connItemModel->rowCount();
                ConnItemIndex *idx = nullptr;
                for (int i=0; i<count; i++) {
                    idx = wdg->connItemModel->connItemDataList.at(i);
                    if ( idx->getName()==_name ) {
                        int row = wdg->connItemModel->connItemDataList.indexOf(idx);
                        wdg->connItemModel->removeRow(row);
                        wdg->connItemModel->connItemDataList.removeOne(idx);
                        break;
                    };
                };
            };
            wdg->connections->remove(key);
        };
        msleep(PERIOD);
    };
}
