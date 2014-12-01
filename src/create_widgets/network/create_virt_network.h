#ifndef CREATE_VIRT_NETWORK_H
#define CREATE_VIRT_NETWORK_H

#include <QDialog>
#include <QSettings>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include "network_widgets/bridge_widget.h"
#include "network_widgets/domain_widget.h"
#include "network_widgets/forward_widget.h"
#include <QDebug>

#define FORWARD_TYPE QStringList()\
    <<"network"\
    <<"bridge"\
    <<"hostdev"\
    <<"mcast"\
    <<"server"\
    <<"user"\
    <<"direct"\
    <<"ethernet"

class CreateVirtNetwork : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(QWidget *parent = NULL);
    ~CreateVirtNetwork();

signals:

private:
    QSettings        settings;
    Bridge_Widget   *bridgeWdg;
    Domain_Widget   *domainWdg;
    Forward_Widget  *forwardWdg;
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
    QString          getXMLDescFileName() const;

private slots:
    void             buildXMLDescription();
    void             set_Result();

};

#endif // CREATE_VIRT_NETWORK_H
