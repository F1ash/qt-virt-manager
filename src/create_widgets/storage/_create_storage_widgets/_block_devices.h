#ifndef _BLOCK_DEVICES_H
#define _BLOCK_DEVICES_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>

class _BlockDevices : public QWidget
{
    Q_OBJECT
public:
    explicit _BlockDevices(QWidget *parent = NULL);
    QLineEdit       *name;

private:
    QListWidget     *devices;
    QPushButton     *add, *browse, *del;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;
    QVBoxLayout     *commonLayout;

    bool             oneDeviceMode = false;

public slots:
    QStringList getDevicesList() const;
    void        setOneDeviceMode(bool);

private slots:
    void addDevice();
    void delDevice();
    void setBlockDevPath();
};

#endif // _BLOCK_DEVICES_H
