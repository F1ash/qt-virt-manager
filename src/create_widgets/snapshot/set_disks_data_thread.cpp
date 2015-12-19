#include "set_disks_data_thread.h"

SetDisksDataThread::SetDisksDataThread(QObject *parent) :
    ControlThread(parent)
{
    qRegisterMetaType<QDomElement>("QDomElement&");
}

SetDisksDataThread::~SetDisksDataThread()
{
    wait(30000);
}

void SetDisksDataThread::run()
{
    if ( NULL==ptr_ConnPtr || NULL==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    // NOTE: currConnName == domainName
    virDomainPtr domain = virDomainLookupByName(
                *ptr_ConnPtr, currConnName.toUtf8().data());
    if ( NULL!=domain ) {
        char *xmlDesc = virDomainGetXMLDesc(domain, 0);
        virDomainFree(domain);
        if ( NULL!=xmlDesc ) {
            QDomDocument doc;
            doc.setContent(QString(xmlDesc));
            free(xmlDesc);
            QDomElement _devices = doc
                    .firstChildElement("domain")
                    .firstChildElement("devices");
            QDomElement _disk = _devices
                    .firstChildElement("disk");
            while ( !_disk.isNull() ) {
                emit diskData(_disk);
                _disk = _disk.nextSiblingElement("disk");
            };
        };
    } else
        sendConnErrors();
}
