#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

class Encryption : public QWidget
{
    Q_OBJECT
public:
    explicit Encryption(
            QWidget         *parent = NULL,
            virConnectPtr    _conn = NULL);

private:
    QCheckBox           *useEncryption;
    QLabel              *formatLabel;
    QComboBox           *format;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg;
    QVBoxLayout         *commonLayout;

    virConnectPtr        currConnection;

public slots:
    bool                 isUsed() const;
    QString              getFormat() const;
};

#endif // ENCRYPTION_H
