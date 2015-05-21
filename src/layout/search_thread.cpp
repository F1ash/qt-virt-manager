#include "search_thread.h"

SearchThread::SearchThread(QObject *parent) :
    QThread(parent)
{
    qRegisterMetaType<QString>("QString&");
    setURIList();
}
SearchThread::~SearchThread()
{
    wait(30000);
}

void SearchThread::run()
{
    //qDebug()<<URIs;
    foreach (QString uri, URIs) {
        virConnect *conn = virConnectOpenReadOnly(uri.toUtf8().data());
        if ( NULL!=conn ) {
            // don't work for VBox
            // int num = virConnectNumOfDefinedDomains(conn);
            virDomainPtr *domains;
            int ret = virConnectListAllDomains(conn, &domains, 0);
            if ( ret+1 ) {
                for (int i = 0; i < ret; i++) {
                     virDomainFree(domains[i]);
                };
                free(domains);
            };
            virConnectClose(conn);
            if ( ret ) {
                emit localConnFound(uri);
            };
        };
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
        <<"openvz:///system";
}
void SearchThread::compareURI(QString &uri)
{
    //qDebug()<<uri;
    URIs.removeAll(uri);
}
