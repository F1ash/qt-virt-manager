#ifndef DIR_DISK_H
#define DIR_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "_disk.h"

class Dir_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Dir_Disk(
            QWidget        *parent     = nullptr,
            virConnectPtr*  connPtrPtr = nullptr);

private:
    QPushButton     *browse;
    QLineEdit       *path;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             setDirPath();
};

#endif // DIR_DISK_H
