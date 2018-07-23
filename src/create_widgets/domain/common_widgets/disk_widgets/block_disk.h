#ifndef BLOCK_DISK_H
#define BLOCK_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "_disk.h"

class Block_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Block_Disk(
            QWidget        *parent     = Q_NULLPTR,
            virConnectPtr*  connPtrPtr = Q_NULLPTR);

private:
    QPushButton     *browse;
    QLineEdit       *path;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);

private slots:
    void             setDevPath();
};

#endif // BLOCK_DISK_H
