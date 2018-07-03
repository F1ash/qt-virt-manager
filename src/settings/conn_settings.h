#ifndef CONN_SETTINGS_H
#define CONN_SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include "virt_objects/virt_connect/connection_list/conn_item_model.h"
//#include <QDebug>

class ConnSettings : public QDialog
{
    Q_OBJECT
public:
    explicit ConnSettings(QWidget *parent = nullptr);
    QSettings            settings;

signals:
    void                 creationConnCancelled();

private:
    ConnItemIndex       *own_index;
    QString              name;
    QString              previousName;

    QVBoxLayout         *commonLayout;

    QLabel              *connName;
    QLineEdit           *ConnName;
    QCheckBox           *atStart;
    QLabel              *driver;
    QComboBox           *Drivers;
    QLabel              *transport;
    QComboBox           *Transports;
    QLabel              *host;
    QLineEdit           *Host;
    QLabel              *path;
    QLineEdit           *Path;
    QLabel              *extra;
    QLineEdit           *Extra;
    QGridLayout         *paramLayout;
    QWidget             *parameters;

    QHBoxLayout         *buttonsLayout;
    QWidget             *buttons;
    QPushButton         *ok;
    QPushButton         *cancel;

    QLabel              *warning;
    QTextEdit           *URI;

    bool                 newbe;

public slots:
    void                 setConnectItem(ConnItemIndex*);

private slots:
    void                 initParamLayout();
    void                 initButtons();
    void                 initParameters();
    void                 saveConnection();
    void                 cancelConnection();
    void                 saveParameters();
    void                 closeEvent(QCloseEvent*);
    void                 set_Title_Name(QString);
    void                 changeURI();
    void                 changeDriver(QString);
    QString              getIconName(QString&) const;
    QString              getDriverName(QString&) const;
};

#endif   // CONN_SETTINGS_H
