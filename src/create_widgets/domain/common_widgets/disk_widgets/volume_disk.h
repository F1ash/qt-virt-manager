#ifndef VOLUME_DISK_H
#define VOLUME_DISK_H

#include <QPushButton>
#include "_disk.h"

class Volume_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Volume_Disk(
            QWidget *parent = 0,
            virConnectPtr conn = NULL);

private:
    QLabel          *poolLabel, *modeLabel;
    QPushButton     *volumeLabel;
    QComboBox       *mode;
    QLineEdit       *pool, *volume;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void changeModeVisibility(QString);
};

#endif // VOLUME_DISK_H
