#ifndef _STORAGE_AUTH_H
#define _STORAGE_AUTH_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include "find_secret_dialog.h"
#include "create_widgets/domain/_qwidget.h"

#define NOT_VOLUME QString("NOT_VOLUME")

class _Storage_Auth : public _Changed
{
    Q_OBJECT
public:
    explicit _Storage_Auth(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL,
            QString         _type   = NOT_VOLUME);
    QCheckBox       *auth;
    QLineEdit       *userName, *usage;
    QComboBox       *usageType;

signals:
    //void             dataChanged();

private:
    QString          secType;
    QLabel          *userLabel;
    QPushButton     *findSecret;
    QGridLayout     *authLayout;
    QWidget         *authWdg;
    QVBoxLayout     *commonLayout;

    virConnectPtr*   ptr_ConnPtr;

public slots:
    void             setSecretType(const QString&);
    QString          getSecretType() const;

private slots:
    void             usageTypeChanged(int);
    void             setSecret();
    void             changeAuthVisibility(int);

};

#endif // _STORAGE_AUTH_H
