#ifndef _SNAPSHOT_STUFF_H
#define _SNAPSHOT_STUFF_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include "_disks.h"
#include "set_disks_data_thread.h"
#include <QDebug>

class _SnapshotStuff : public QWidget
{
    Q_OBJECT
public:
    explicit _SnapshotStuff(
            QWidget *parent = NULL,
            bool     _state = false);
    const bool               state;
    QLabel                  *memLabel, *filePathLabel;
    QLineEdit               *filePath;
    QCheckBox               *diskWarn;
    _Disks                  *disks;
    QVBoxLayout             *commonLayout;
    virtual QDomDocument     getElements() const;
    void                     setParameters(virConnectPtr, QString&);

signals:
    void                     errMsg(QString&);
};

#endif // _SNAPSHOT_STUFF_H
