#ifndef CREATE_VOLUME_H
#define CREATE_VOLUME_H

#include <QSpinBox>
#include "_create_storage_widgets/_create_storage.h"
//#include "_create_storage_widgets/_storage_source.h"
#include "_create_storage_widgets/_storage_target.h"
#include "virt_objects/_virt_thread.h"

class pooHelperThread : public _VirtThread
{
    Q_OBJECT
public:
    explicit pooHelperThread(
            QObject        *parent      = Q_NULLPTR,
            virConnectPtr  *connPtrPtr  = Q_NULLPTR,
            QString         _name       = QString());
    virStoragePool      *pool;
    QString              name, type;
    void                 run();
};

class CreateVolume : public _CreateStorage
{
    Q_OBJECT
public:
    explicit CreateVolume(
            QWidget         *parent      = Q_NULLPTR,
            virConnectPtr   *connPtrPtr  = Q_NULLPTR,
            QString          _poolName   = QString(),
            QString          _xmlFile    = QString());
    void             setDataDescription(const QString&);

signals:

private:
    QComboBox       *capLabel, *allocLabel;
    QSpinBox        *capacity, *allocation;
    QGridLayout     *sizeLayout;
    QWidget         *sizeWdg;
    //_Storage_Source *source;
    _Storage_Target *target;

    QVBoxLayout     *infoStuffLayout;
    QWidget         *infoStuff;
    pooHelperThread *hlpThread;

public slots:
    QString          getXMLDescFileName() const;

private slots:
    void             initData();
};

#endif // CREATE_VOLUME_H
