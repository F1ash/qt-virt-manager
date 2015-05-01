#include "set_disks_data_thread.h"

SetDisksDataThread::SetDisksDataThread(QObject *parent) :
    ControlThread(parent)
{
    qRegisterMetaType<QDomElement>("QDomElement&");
}

SetDisksDataThread::~SetDisksDataThread()
{

}

void SetDisksDataThread::run()
{
    // NOTE: currConnName == domanName
    virDomainPtr domain = virDomainLookupByName(
                currWorkConnect, currConnName.toUtf8().data());
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
                _disk = _devices.nextSiblingElement("disk");
            };
        };
    } else
        sendConnErrors();
}
