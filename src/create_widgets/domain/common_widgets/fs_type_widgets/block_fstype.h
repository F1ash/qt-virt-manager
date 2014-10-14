#ifndef BLOCK_FSTYPE_H
#define BLOCK_FSTYPE_H

#include "_fstype.h"

class BlockFsType : public _FsType
{
    Q_OBJECT
public:
    explicit BlockFsType(
            QWidget *parent = NULL,
            QString _type = "lxc");

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void getSourcePath();
    void driverTypeChanged(QString);
};

#endif // BLOCK_FSTYPE_H
