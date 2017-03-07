#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <libvirt.h>

class Encryption : public QWidget
{
    Q_OBJECT
public:
    explicit Encryption(
            QWidget         *parent     = nullptr,
            virConnectPtr   *connPtrPtr = nullptr);
    bool                 isUsed() const;
    void                 setUsage(const bool);
    QString              getFormat() const;
    void                 setFormat(const QString&);
    bool                 AutoSecretIsUsed() const;
    void                 setAutoSecretUsage(const bool);
    QString              getSecretUUID() const;
    void                 setSecretUUID(const QString&);

private:
    QCheckBox           *useEncryption, *autoSecret;
    QPushButton         *findSecret;
    QLabel              *formatLabel, *secUUID;
    QComboBox           *format;
    QHBoxLayout         *secretLayout;
    QGridLayout         *baseLayout;
    QWidget             *baseWdg, *secretWdg;
    QVBoxLayout         *commonLayout;

    virConnectPtr*       ptr_ConnPtr;

private slots:
    void                 setSecret();
};

#endif // ENCRYPTION_H
