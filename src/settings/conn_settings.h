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
#include "layout/conn_item_model.h"
#include <QDebug>

class ConnSettings : public QDialog
{
    Q_OBJECT
public:
    ConnSettings(QWidget *parent=0);
    ~ConnSettings();
    QSettings    settings;

signals:
    void creationConnCancelled();

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
    int                  timerId;

public slots:
    void                 setConnectItem(ConnItemIndex*);

private slots:
    void                 initParamLayout();
    void                 initButtons();
    void                 initParameters();
    void                 saveConnect();
    void                 cancelConnect();
    void                 saveParameters();
    void                 closeEvent(QCloseEvent*);
    void                 set_Title_Name(QString);
    void                 timerEvent(QTimerEvent*);
    void                 changeConnParameters(QString);
    QString              getIconName(QString&) const;
    QString              getDriverName(QString&) const;
};

#endif   // CONN_SETTINGS_H
