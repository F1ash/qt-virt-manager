#ifndef ADAPTER_ADDRESS_H
#define ADAPTER_ADDRESS_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>

class AdapterAddress : public QWidget
{
    Q_OBJECT
public:
    explicit AdapterAddress(QWidget *parent = Q_NULLPTR);
    QCheckBox           *adapterAddr;
    QSpinBox            *bus, *target, *unit;

signals:
    void                 dataChanged();

private:
    QLabel              *busLabel, *targetLabel,
                        *unitLabel;
    QGridLayout         *scsiAddrLayout;
    QWidget             *scsiAddr;
    QHBoxLayout         *commonLayout;

public slots:
    void                 stateChanged();
};

#endif // ADAPTER_ADDRESS_H
