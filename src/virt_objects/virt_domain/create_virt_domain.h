#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QDialog>
#include <QDir>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtXml/QDomDocument>
#include <QTextStream>
#include "general.h"
#include <QDebug>

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(QWidget *parent = 0, QString str = "");
    ~CreateVirtDomain();

signals:

private:
    QString          type;
    QTabWidget      *tabWidget;
    General         *general;
    QWidget         *boot;
    QWidget         *cpu;
    QWidget         *memory;
    QWidget         *power;
    QWidget         *devices;
    QWidget         *interfaces;
    QWidget         *security;
    QPushButton     *ok;
    QPushButton     *cancel;
    QWidget         *buttons;
    QHBoxLayout     *buttonLayout;
    QVBoxLayout     *commonLayout;

    QTemporaryFile  *xml;

public slots:
    QString getXMLDescFileName() const;

private slots:
    void buildXMLDescription();
    void set_Result();

};

#endif // CREATE_VIRT_DOMAIN_H
