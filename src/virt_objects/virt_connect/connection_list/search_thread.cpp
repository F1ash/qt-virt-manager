#include "search_thread.h"

SearchThread::SearchThread(QObject *parent) :
    _VirtThread(parent)
{
    qRegisterMetaType<QString>("QString&");
    setURIList();
}

void SearchThread::run()
{
    //qDebug()<<URIs;
    foreach (QString uri, URIs) {
        virConnect *connPtr =
                virConnectOpenReadOnly(uri.toUtf8().data());
        if ( nullptr!=connPtr ) {
            // don't work for VBox
            // int num = virConnectNumOfDefinedDomains(connPtr);
            virDomainPtr *domains;
            int ret = virConnectListAllDomains(
                        connPtr, &domains, 0);
            if ( ret+1 ) {
                for (int i = 0; i < ret; i++) {
                     if ( virDomainFree(domains[i]) <0 )
                         sendConnErrors();
                };
                free(domains);
            } else {
                sendConnErrors();
            };
            virConnectClose(connPtr);
            if ( ret ) {
                emit localConnFound(uri);
            };
        } else {
            sendConnErrors();
        };
        //msleep(333);
    };
}
void SearchThread::setURIList()
{
    URIs.clear();
    URIs<<"lxc:///"\
        <<"qemu:///system"\
        <<"qemu:///session"\
        <<"xen:///"\
        <<"vbox:///session"\
        <<"vmwareplayer:///session"\
        <<"vmwarews:///session"\
        <<"vmwarefusion:///session"\
        <<"esx:///"\
        <<"gsx:///"\
        <<"vpx:///"\
        <<"openvz:///system";
}
void SearchThread::compareURI(QString &uri)
{
    //qDebug()<<uri;
    URIs.removeAll(uri);
}
