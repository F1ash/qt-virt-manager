#ifndef BLOCK_FSTYPE_H
#define BLOCK_FSTYPE_H

#include "_fstype.h"

class BlockFsType : public _FsType
{
    Q_OBJECT
public:
    explicit BlockFsType(
            QWidget *parent = nullptr,
            QString _type = "lxc");

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             getSourcePath();
};

#endif // BLOCK_FSTYPE_H
