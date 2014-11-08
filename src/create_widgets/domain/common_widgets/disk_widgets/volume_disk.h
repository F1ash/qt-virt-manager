#ifndef VOLUME_DISK_H
#define VOLUME_DISK_H

#include <QPushButton>
#include "_disk.h"
#include "virtvolume_dialog.h"

class Volume_Disk : public _Disk
{
    Q_OBJECT
public:
    explicit Volume_Disk(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL);

private:
    QLabel          *poolLabel, *modeLabel,
                    *pool;
    QPushButton     *volumeLabel;
    QLineEdit       *volume;
    QComboBox       *mode;
    VirtVolumeDialog
                    *volumeDialog = NULL;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(QString&);

private slots:
    void             changeModeVisibility(QString);
    void             getVolumeNames();
};

#endif // VOLUME_DISK_H
