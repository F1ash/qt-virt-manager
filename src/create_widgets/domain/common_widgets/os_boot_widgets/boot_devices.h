#ifndef BOOT_DEVICES_H
#define BOOT_DEVICES_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QDomDocument>
#include <QDebug>

class Boot_Devices : public QWidget
{
    Q_OBJECT
public:
    explicit Boot_Devices(QWidget *parent = 0);
    QListWidget     *devices;

signals:
    void             dataChanged();

private:
    QPushButton     *up, *down;
    QGridLayout     *commonLayout;

public slots:
    void             addNewDevice(QDomElement&);

private slots:
    void             itemUp();
    void             itemDown();
    void             orderChanged(QListWidgetItem*);
};

#endif // BOOT_DEVICES_H
