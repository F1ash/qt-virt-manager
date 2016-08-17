#ifndef _BLOCK_DEVICES_H
#define _BLOCK_DEVICES_H

#include "create_widgets/common/_list_widget.h"
#include <QFileDialog>

class _BlockDevices : public _List_Widget
{
    Q_OBJECT
public:
    explicit _BlockDevices(
            QWidget *parent = nullptr,
            QString  tag    = "");
    QLineEdit       *name;

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

#endif // _BLOCK_DEVICES_H
