#ifndef _CREATE_STORAGE_H
#define _CREATE_STORAGE_H

#include <QDialog>
#include <QScrollArea>
#include <QStackedWidget>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QtXml/QDomDocument>
#include <QStringList>
#include <QDir>
#include <QTemporaryFile>
#include <QSettings>
extern "C" {
#include <libvirt.h>
}
//#include <QDebug>

class _CreateStorage : public QDialog
{
    Q_OBJECT
public:
    explicit _CreateStorage(
            QWidget         *parent     = Q_NULLPTR,
            virConnectPtr   *connPtrPtr = Q_NULLPTR,
            QString          _xmlFile   = QString());
    QComboBox           *type;
    QLineEdit           *stName;
    QLabel              *suff;
    QVBoxLayout         *commonLayout;
    QTemporaryFile      *xml;
    QStackedWidget      *info;
    QScrollArea         *infoWidget;

    QSettings            settings;
    QString              settingName;
    QCheckBox           *showAtClose;
    void                 readXMLDataDescription();
    virtual void         setDataDescription(const QString&);
    quint64              convertNiBtoMiB(quint64, const QString&);

    virConnectPtr       *ptr_ConnPtr;

signals:
    void                 errorMsg(const QString&);

private:
    const QString        xmlFileName;
    QLabel              *typeLabel, *stNameLabel;
    QLabel              *about;
    QPushButton         *chooseStorage;
    QPushButton         *cancel;
    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;

    QGridLayout         *baseLayout;
    QWidget             *baseWdg;

    QVBoxLayout         *infoLayout;
    QWidget             *scrolled;

public slots:
    virtual QString      getXMLDescFileName() const;
    bool                 showXMLDescription() const;
    void                 setUrl(QString);

private slots:
    void                 set_Result();
};

#endif // _CREATE_STORAGE_H
