#ifndef _DEVICE_LIST_H
#define _DEVICE_LIST_H

#include "create_widgets/common/_list_widget.h"
#include <QFileDialog>

class _DeviceList : public _List_Widget
{
    Q_OBJECT
public:
    explicit _DeviceList(
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "");
    QLineEdit       *name;
    void             addNewDevicePath(const QString&);

private:
    QPushButton     *browse;
    bool             oneDeviceMode = false;

public slots:
    QStringList      getDevicesList() const;
    void             setOneDeviceMode(bool);

private slots:
    void             addItem();
    void             delItem();
    void             setBlockDevPath();
};

#endif // _DEVICE_LIST_H
