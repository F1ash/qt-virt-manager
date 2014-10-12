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
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QPushButton     *browse;
    QLineEdit       *path;

public slots:
    QDomDocument getDataDocument() const;

private slots:
    void setDevPath();
};

#endif // BLOCK_DISK_H
