#ifndef FILE_DISK_H
#define FILE_DISK_H

#include <QFileDialog>
#include <QPushButton>
#include "_disk.h"

class File_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit File_Disk(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QPushButton     *browse;
    QLineEdit       *path;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             setFilePath();
};

#endif // FILE_DISK_H
