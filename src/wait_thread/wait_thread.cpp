#include "wait_thread.h"

Wait::Wait(QObject *parent = 0) :
    QThread(parent)
{
    processingState = false;
}
Wait::~Wait()
{

}
void Wait::run()
{
  while (wdg->connItemModel->connItemDataList.count()) {
      if ( processingState ) {
          int count = wdg->connItemModel->connItemDataList.count();
          QList<QString> to_Delete;
          for (int i=0; i<count; i++) {
              ConnItemIndex *idx = wdg->connItemModel->connItemDataList.at(i);
              DATA map = idx->getData();
              if ( map.value("availability").toBool() && !map.value("isRunning").toBool() ) {
                  to_Delete.append(idx->getName());
              } else if ( map.value("availability").toBool() && map.value("isRunning").toBool() ) {
                  wdg->connects->value(idx->getName())->closeConnect();
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
      };
      emit refreshProcessingState();
      msleep(500);
  };
  msleep(1000);
}
void Wait::setPtr(ConnectList *w)
{
    wdg = w;
}
void Wait::setProcessingState(bool b)
{
    processingState = b;
}
