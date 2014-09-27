#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QDialog>
#include <QList>
#include <QSettings>
#include <QDir>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QPushButton>
#include <QTextStream>
#include <QTimerEvent>
#include "domain_widgets.h"
#include "common_widgets/devices.h"
#include <QDebug>

typedef QList<_QWidget*> WidgetList;

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(
            QWidget *parent = 0,
            virConnectPtr conn = NULL,
            QString _xmlFileName = QString());
    ~CreateVirtDomain();

signals:
    void errorMsg(QString);

private:
    QSettings        settings;
    virConnect      *currWorkConnect = NULL;
    QString          xmlFileName, xmlDesc;
    virErrorPtr      virtErrors;
    QString          capabilities;
    QString          type;
    QString          arch;
    QString          os_type;
    QString          memUnit;
    QString          memValue;
    QTabWidget      *tabWidget = NULL;
    QCheckBox       *showDescription;
    QPushButton     *ok;
    QPushButton     *restore;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml = NULL;
    WidgetList       wdgList;
    bool             ready = false;
    uint             timerId = 0;
    uint             counter = 0;

public slots:
    QString getXMLDescFileName() const;
    bool getShowing() const;

private slots:
    void readCapabilities();
    void readyDataLists();
    void timerEvent(QTimerEvent*);
    void buildXMLDescription();
    void set_Result();
    void create_specified_widgets();
    void set_specified_Tabs();
    void delete_specified_widgets();
    void restoreParameters();

    void sendConnErrors();
    void sendGlobalErrors();

};

#endif // CREATE_VIRT_DOMAIN_H
