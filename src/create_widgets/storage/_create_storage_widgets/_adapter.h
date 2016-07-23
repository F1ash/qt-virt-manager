#ifndef _ADAPTER_H
#define _ADAPTER_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include "create_widgets/domain/address_widgets/pci_addr.h"

class _Adapter : public QWidget
{
    Q_OBJECT
public:
    explicit _Adapter(QWidget *parent = nullptr);
    QLineEdit           *name, *wwnn, *wwpn,
                        *adapterPrnt, *adapterPrntID;
    QComboBox           *type;
    QCheckBox           *usePrntAddr;
    PciAddr             *addr;

private:
    QLabel              *nameLabel, *typeLabel,
                        *wwnnLabel, *wwpnLabel,
                        *adapterPrntLabel,
                        *adapterPrntIDLabel;
    QGridLayout         *commonLayout;

public slots:

private slots:
    void typeAdapterChanged(QString);

private slots:
    void usingPrntAddrChanged(bool);
};

#endif // _ADAPTER_H
