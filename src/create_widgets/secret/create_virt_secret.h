#ifndef CREATE_VIRT_SECRET_H
#define CREATE_VIRT_SECRET_H

#include <QDialog>
#include <QSettings>
#include <QScrollArea>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QDomDocument>
#include <QStackedWidget>
#include "virt_objects/virt_entity_config.h"
#include "secret_widgets/ceph_sec_type.h"
#include "secret_widgets/iscsi_sec_type.h"
#include "secret_widgets/volume_sec_type.h"
#include <QDebug>

class CreateVirtSecret : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtSecret(
            QWidget         *parent        = nullptr,
            virConnectPtr*   connPtrPtr    = nullptr);
    ~CreateVirtSecret();

signals:
    void             errorMsg(QString);

private:
    QSettings        settings;
    QString          UUID;
    virConnectPtr*   ptr_ConnPtr;

    QScrollArea     *scroll;
    QWidget         *scrolled;
    QVBoxLayout     *scrollLayout;
    QWidget         *baseWdg;
    QGridLayout     *baseLayout;

    QLineEdit       *uuid, *secDesc, *secValue;
    QComboBox       *secType;
    QCheckBox       *ephemeralAttr, *privateAttr;
    QHBoxLayout     *propLayout;
    QWidget         *propWdg;
    QStackedWidget  *stuffWdg;
    QCheckBox       *showXMLDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
    QWidget         *buttons;

    QVBoxLayout     *secDescLayout;

    QTemporaryFile  *xml;

public slots:
    QString          getXMLDescFileName() const;
    QByteArray       getSecretValue() const;
    bool             getShowing() const;

private slots:
    void             buildXMLDescription();
    void             set_Result();
};

#endif // CREATE_VIRT_SECRET_H
