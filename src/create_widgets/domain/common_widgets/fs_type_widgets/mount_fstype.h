#ifndef MOUNT_FSTYPE_H
#define MOUNT_FSTYPE_H

#include "_fstype.h"

class MountFsType : public _FsType
{
    Q_OBJECT
public:
    explicit MountFsType(
            QWidget *parent = nullptr,
            QString _type = QString());

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             getSourcePath();
};

#endif // MOUNT_FSTYPE_H
