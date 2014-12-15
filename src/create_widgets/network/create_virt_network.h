#ifndef CREATE_VIRT_NETWORK_H
#define CREATE_VIRT_NETWORK_H

#include <QDialog>
#include <QSettings>
#include <QScrollArea>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include "network_widgets/bridge_widget.h"
#include "network_widgets/domain_widget.h"
#include "network_widgets/forward_widget.h"
#include <QDebug>

class CreateVirtNetwork : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtNetwork(QWidget *parent = NULL);
    ~CreateVirtNetwork();

signals:
    void             errorMsg(QString);

private:
    QSettings        settings;
    QScrollArea     *scroll;
    QWidget         *scrolled;
    QVBoxLayout     *scrollLayout;
    QWidget         *baseWdg;
    QLabel          *netNameLabel, *uuidLabel;
    QLineEdit       *networkName, *uuid;
    QCheckBox       *ipv6, *trustGuestRxFilters;
    QGridLayout     *baseLayout;
    Bridge_Widget   *bridgeWdg;
    Domain_Widget   *domainWdg;
    Forward_Widget  *forwardWdg;

    QCheckBox       *showDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
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
};

#endif // CREATE_VIRT_NETWORK_H
