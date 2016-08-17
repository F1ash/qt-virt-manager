#ifndef CREATE_VIRT_NETWORK_ADV_H
#define CREATE_VIRT_NETWORK_ADV_H

#include <QWidget>
#include <QSettings>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include "network_widgets/bridge_widget.h"
#include "network_widgets/domain_widget.h"
#include "network_widgets/forward_widget.h"
#include "network_widgets/addressing_widget.h"
#include "network_widgets/qos_widget.h"
#include <QDebug>

class CreateVirtNetwork_Adv : public QWidget
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork_Adv(
            QWidget *parent = nullptr);
    ~CreateVirtNetwork_Adv();
    void             readXmlDescData(const QString&);

signals:
    void             errorMsg(const QString&);
    void             newName(const QString&);
    void             accepted(bool);

private:
    QSettings        settings;
    QWidget         *baseWdg, *noGWWdg;
    QLabel          *netNameLabel, *uuidLabel;
    QLineEdit       *networkName, *uuid;
    QCheckBox       *noGW, *ipv6, *trustGuestRxFilters;
    QGridLayout     *baseLayout;
    Bridge_Widget   *bridgeWdg;
    Domain_Widget   *domainWdg;
    Addressing_Widget
                    *addressingWdg;
    Forward_Widget  *forwardWdg;
    QoS_Widget      *QoSWdg;
    QTabWidget      *tabs;

    //QCheckBox       *showDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout, *noGWLayout;
    QWidget         *buttons;
    QVBoxLayout     *netDescLayout;

    QTemporaryFile  *xml;

public slots:
    QString          getXMLDescFileName() const;
    bool             getShowing() const;

private slots:
    void             buildXMLDescription();
    void             set_Result();
    void             networkTypeChanged(bool);
    void             noGatewayChanged(bool);
};

#endif // CREATE_VIRT_NETWORK_ADV_H
