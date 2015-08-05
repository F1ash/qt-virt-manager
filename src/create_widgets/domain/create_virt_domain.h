#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QDialog>
#include <QMap>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QPushButton>
#include "domain_widgets.h"
#include "common_widgets/devices.h"
#include <QDebug>

typedef QMap<QString, _QWidget*> WidgetList;

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(
            QWidget *parent = NULL,
            virConnectPtr conn = NULL,
            QString _xmlFileName = QString(),
            Actions _act         = _EMPTY_ACTION);
    ~CreateVirtDomain();

signals:
    void             errorMsg(QString&);
    void             readyRead(bool);

private:
    QSettings        settings;
    virConnect      *currWorkConnection = NULL;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;
    QString          capabilities;
    QString          type;
    QTabWidget      *tabWidget = NULL;
    QCheckBox       *showDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *restore;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml = NULL;
    WidgetList       wdgList;
    bool             ready = false;
    Actions          action;

public slots:
    int              getResult() const;
    Actions          getAction() const;
    QString          getXMLDescFileName() const;
    bool             getShowing() const;

private slots:
    void             readCapabilities();
    void             readyDataLists();
    void             buildXMLDescription();
    void             set_Result();
    void             create_specified_widgets();
    void             set_specified_Tabs();
    void             delete_specified_widgets();
    void             restoreParameters();
    void             setBootOrder(QDomElement*);
};

#endif // CREATE_VIRT_DOMAIN_H
