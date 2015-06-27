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
#include "virt_objects/virt_entity_config.h"
#include <QDebug>

class CreateVirtSecret : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtSecret(
            QWidget         *parent = NULL,
            virConnectPtr    conn = NULL,
            QString          _uuid = QString());
    ~CreateVirtSecret();

signals:
    void             errorMsg(QString);

private:
    QSettings        settings;
    QString          UUID;
    virConnectPtr    currConnection;

    QScrollArea     *scroll;
    QWidget         *scrolled;
    QVBoxLayout     *scrollLayout;
    QWidget         *baseWdg;
    QGridLayout     *baseLayout;

    QLineEdit       *uuid, *secDesc;
    QComboBox       *secType;
    QCheckBox       *ephemeralAttr, *privateAttr;
    QHBoxLayout     *propLayout;
    QWidget         *propWdg;
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
    bool             getShowing() const;

private slots:
    void             buildXMLDescription();
    void             set_Result();
    void             secretTypeChanged(QString);
};

#endif // CREATE_VIRT_SECRET_H
