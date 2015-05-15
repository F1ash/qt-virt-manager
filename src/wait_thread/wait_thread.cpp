#include "wait_thread.h"
#define  PERIOD 333

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
    while (wdg->connItemModel->connItemDataList.count()) {
        int count = wdg->connItemModel->connItemDataList.count();
        QList<QString> to_Delete;
        for (int i=0; i<count; i++) {
            ConnItemIndex *idx = wdg->connItemModel->connItemDataList.at(i);
            if ( NULL==idx ) continue;
            DATA map = idx->getData();
            if ( map.value("availability").toBool() && map.value("isRunning").toInt()!=RUNNING ) {
                to_Delete.append(idx->getName());
            } else if ( map.value("isRunning").toInt()==RUNNING ) {
                wdg->connections->value(idx->getName())->closeConnection();
            };
        };
        ConnItemIndex *idx;
        QList<QString>::const_iterator j;
        for (j=to_Delete.constBegin(); j!=to_Delete.constEnd(); j++) {
            int count = wdg->connItemModel->rowCount();
            bool exist = false;
            for (int i=0; i<count; i++) {
                idx = wdg->connItemModel->connItemDataList.at(i);
                if ( idx->getName()==*j ) {
                    exist = true;
                    break;
                }
            };
            int row = wdg->connItemModel->connItemDataList.indexOf(idx);
            if (exist) wdg->connItemModel->removeRow(row);
        };
        msleep(PERIOD);
    };
    msleep(PERIOD);
}
