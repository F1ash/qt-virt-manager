#ifndef CREATE_VIRT_NETWORK_H
#define CREATE_VIRT_NETWORK_H

#include <QDialog>
#include <QSettings>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include "libvirt/libvirt.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>

#define FORWARD_TYPE QStringList()<<"network"<<"bridge"<<"hostdev"<<"mcast"<<"server"<<"user"<<"direct"<<"ethernet"

class CreateVirtNetwork : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(QWidget *parent = 0);
    ~CreateVirtNetwork();

signals:

private:
    QSettings        settings;
    QWidget         *bridge;
    QGridLayout     *bridgeLayout;
    QLineEdit       *bridgeName;
    QCheckBox       *stp;
    QLabel          *delayLabel;
    QSpinBox        *delay;

    QVBoxLayout     *netDescLayout;
    QLineEdit       *networkName;
    QLabel          *uuid;
    QLineEdit       *source;
    QLineEdit       *target;
    QLineEdit       *address;
    QLineEdit       *mac;

    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
    QWidget         *buttons;

    QTemporaryFile  *xml;

public slots:
    QString getXMLDescFileName() const;

private slots:
    void buildXMLDescription();
    void set_Result();

};

#endif // CREATE_VIRT_NETWORK_H
