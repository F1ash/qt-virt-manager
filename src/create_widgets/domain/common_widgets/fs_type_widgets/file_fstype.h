#ifndef FILE_FSTYPE_H
#define FILE_FSTYPE_H

#include "_fstype.h"

class FileFsType : public _FsType
{
    Q_OBJECT
public:
    explicit FileFsType(
            QWidget *parent = NULL,
            QString _type = "lxc");

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void getSourcePath();
    void driverTypeChanged(QString);
};

#endif // FILE_FSTYPE_H
