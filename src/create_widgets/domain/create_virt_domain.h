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
#include "domain_widgets.h"
#include "_qwidget.h"
#include <QDebug>

typedef QList<_QWidget*> WidgetList;

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(QWidget *parent = 0, QString str = "");
    ~CreateVirtDomain();

signals:

private:
    QSettings        settings;
    QString          type;
    QTabWidget      *tabWidget;
    QPushButton     *ok;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml;
    WidgetList       wdgList;

public slots:
    QString getXMLDescFileName() const;

private slots:
    void buildXMLDescription();
    void set_Result();
    void create_specified_widgets();
    void set_specified_Tabs();
    void delete_specified_widgets();

};

#endif // CREATE_VIRT_DOMAIN_H
