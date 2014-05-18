#include "wait_thread.h"

Wait::Wait(QObject *parent, ConnectList *wdgList, ViewerMap map) :
    QThread(parent), wdg(wdgList), vm_displayed_map(map)
{
    processingState = false;
}
Wait::~Wait()
{

}
void Wait::run()
{
  // close VM Displays
  foreach ( QString key, vm_displayed_map.keys() ) {
      VM_Viewer *vm = vm_displayed_map.value(key, NULL);
      bool state = (vm!=NULL)? vm->isActive() : false;
      if ( state ) vm->stopProcessing();
  };
  //vm_displayed_map.clear();
  // close connections
  while (wdg->connItemModel->connItemDataList.count()) {
      if ( processingState ) {
          int count = wdg->connItemModel->connItemDataList.count();
          QList<QString> to_Delete;
          for (int i=0; i<count; i++) {
              ConnItemIndex *idx = wdg->connItemModel->connItemDataList.at(i);
              DATA map = idx->getData();
              if ( map.value("availability").toBool() && map.value("isRunning").toInt()!=RUNNING ) {
                  to_Delete.append(idx->getName());
              } else if ( map.value("availability").toBool() && map.value("isRunning").toInt()==RUNNING ) {
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
      msleep(333);
  };
  msleep(333);
}
void Wait::setProcessingState(bool b)
{
    processingState = b;
}
