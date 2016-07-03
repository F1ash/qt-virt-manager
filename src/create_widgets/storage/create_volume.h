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
            QObject        *parent      = nullptr,
            virConnectPtr  *connPtrPtr  = nullptr,
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
            QWidget         *parent      = nullptr,
            virConnectPtr   *connPtrPtr  = nullptr,
            QString          _poolName   = QString());

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
    pooHelperThread *helperThread;

public slots:
    QString          getXMLDescFileName() const;

private slots:
    void             initData();
};

#endif // CREATE_VOLUME_H
