#ifndef RAM_FSTYPE_H
#define RAM_FSTYPE_H

#include "_fstype.h"

class RAMFsType : public _FsType
{
    Q_OBJECT
public:
    explicit RAMFsType(
            QWidget *parent = NULL,
            QString _type = "lxc");

private:
    QSpinBox        *usage;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);
};

#endif // RAM_FSTYPE_H
