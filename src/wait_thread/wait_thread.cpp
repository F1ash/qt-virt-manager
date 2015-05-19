#include "wait_thread.h"
#define  PERIOD 333
#define MINUTE 60000

Wait::Wait(QObject *parent, ConnectionList *wdgList) :
    QThread(parent), wdg(wdgList)
{

}
Wait::~Wait()
{

}
void Wait::run()
{
    // close connections
    int waitTimeout = 0;
    while (wdg->connItemModel->connItemDataList.count()) {
        int count = wdg->connItemModel->connItemDataList.count();
        QStringList to_Delete;
        for (int i=0; i<count; i++) {
            ConnItemIndex *idx = wdg->connItemModel->connItemDataList.at(i);
            if ( NULL==idx ) continue;
            DATA map = idx->getData();
            if ( map.value("availability").toBool() && map.value("isRunning").toInt()!=RUNNING ) {
                to_Delete.append(idx->getName());
            } else if ( map.value("isRunning").toInt()==RUNNING ) {
                ConnElement *el = static_cast<ConnElement*>(
                            wdg->connections->value(idx->getName()));
                if ( NULL!=el ) el->closeConnection();
            };
        };
        foreach (QString _name, to_Delete) {
            int count = wdg->connItemModel->rowCount();
            bool exist = false;
            ConnItemIndex *idx = NULL;
            for (int i=0; i<count; i++) {
                idx = wdg->connItemModel->connItemDataList.at(i);
                if ( idx->getName()==_name ) {
                    exist = true;
                    break;
                }
            };
            if ( NULL!=idx ) {
                int row = wdg->connItemModel->connItemDataList.indexOf(idx);
                if (exist) wdg->connItemModel->removeRow(row);
            };
        };
        msleep(PERIOD);
        waitTimeout += PERIOD;
        if ( waitTimeout>MINUTE ) {
            foreach (QString key, wdg->connections->keys()) {
                ConnElement *el = static_cast<ConnElement*>(
                            wdg->connections->value(key));
                if ( NULL!=el ) el->forceCloseConnection();
            };
        };
    };
    msleep(PERIOD);
}
