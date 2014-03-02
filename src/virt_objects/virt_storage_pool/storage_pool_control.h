#ifndef STORAGE_POOL_CONTROL_H
#define STORAGE_POOL_CONTROL_H

#include <QMainWindow>

class VirtStoragePoolControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtStoragePoolControl(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // STORAGE_POOL_CONTROL_H
