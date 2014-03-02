#ifndef STORAGE_VOL_CONTROL_H
#define STORAGE_VOL_CONTROL_H

#include <QMainWindow>

class VirtStorageVolControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtStorageVolControl(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // STORAGE_VOL_CONTROL_H
