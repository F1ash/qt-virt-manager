#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QDialog>
#include <QList>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QTimerEvent>
#include "domain_widgets.h"
#include "_qwidget.h"
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

typedef QList<_QWidget*> WidgetList;

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(QWidget *parent = 0, virConnectPtr conn = NULL);
    ~CreateVirtDomain();

signals:
    void errorMsg(QString);

private:
    QSettings        settings;
    virConnect      *currWorkConnect = NULL;
    virErrorPtr      virtErrors;
    virNodeDevice  **nodeDevices = NULL;
    QString          capabilities;
    QStringList      nets;
    QStringList      devices;
    QString          type;
    QString          arch;
    QString          os_type;
    QString          emulator;
    QString          memUnit;
    QString          memValue;
    QTabWidget      *tabWidget;
    QPushButton     *ok;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml;
    WidgetList       wdgList;
    bool             ready = false;
    uint             timerId = 0;
    uint             counter = 0;

public slots:
    QString getXMLDescFileName() const;

private slots:
    void readCapabilities();
    void readNetworkList();
    void readNodeDevicesList();
    void readyDataLists();
    void timerEvent(QTimerEvent*);
    void buildXMLDescription();
    void set_Result();
    void create_specified_widgets();
    void set_specified_Tabs();
    void delete_specified_widgets();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // CREATE_VIRT_DOMAIN_H
