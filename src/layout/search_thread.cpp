#include "search_thread.h"

SearchThread::SearchThread(QObject *parent) :
    QThread(parent)
{
    URIs<<"lxc:///"<<"qemu:///system"<<"qemu:///session"<<"xen:///"<<"vbox:///session"<<"openvz:///system";
}
SearchThread::~SearchThread()
{
    wait(30000);
}

void SearchThread::run()
{
    qDebug()<<URIs;
}
void SearchThread::compareURI(QString &uri)
{
    qDebug()<<uri;
    URIs.removeAll(uri);
}
