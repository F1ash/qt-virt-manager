#ifndef CREATE_INTERFACE_H
#define CREATE_INTERFACE_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QDomDocument>
//#include <QDebug>

class CreateInterface : public QDialog
{
    Q_OBJECT
public:
    explicit CreateInterface(QWidget *parent = Q_NULLPTR);
    ~CreateInterface();

private:
    QSettings            settings;

    QComboBox           *type;
    QLineEdit           *name, *ip4Addr, *ip6Addr;
    QSpinBox            *ip4Pref, *ip6Pref;
    QLabel              *ip4, *ip6;
    QGridLayout         *ipLayout;
    QWidget             *protocols;

    QCheckBox           *showXMLDescription;
    QPushButton         *ok;
    QPushButton         *cancel;
    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;
    QVBoxLayout         *commonLayout;

    QTemporaryFile      *xml;

public slots:
    QString              getXMLDescFileName() const;
    bool                 getShowing() const;

private slots:
    void                 buildXMLDescription();
    void                 set_Result();
};

#endif // CREATE_INTERFACE_H
