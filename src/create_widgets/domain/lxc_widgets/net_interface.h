#ifndef LXC_NET_INTERFACE_H
#define LXC_NET_INTERFACE_H

#include "create_widgets/domain/_qwidget.h"

class LXC_NetInterface : public _QWidget
{
    Q_OBJECT
public:
    explicit LXC_NetInterface(QWidget *parent = 0, QStringList nets = QStringList());
    ~LXC_NetInterface();

signals:

private:
    QStringList              existNetwork;
    QCheckBox               *useExistNetwork;
    QComboBox               *networks;

    QLineEdit               *bridgeName;
    QLabel                  *macLabel;
    QLineEdit               *mac;
    QGridLayout             *netDescLayout;
    QWidget                 *netDescWdg;

    QVBoxLayout             *commonLayout;

public slots:
    QDomNodeList getNodeList() const;
    QString      getDevType() const;

private slots:
    void changeUsedNetwork(bool);
    void changeUsedNetwork(QString);

};

#endif // LXC_NET_INTERFACE_H
