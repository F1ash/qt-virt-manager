#ifndef CREATE_VIRT_DOMAIN_H
#define CREATE_VIRT_DOMAIN_H

#include <QDialog>
#include <QTemporaryFile>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class CreateVirtDomain : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtDomain(QWidget *parent = 0);
    ~CreateVirtDomain();

signals:

private:
    QLineEdit       *input;
    QTabWidget      *tabWidget;
    QWidget         *general;
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

public slots:
    QString getXMLFile() const;

private slots:
    void buildXMLDescription();
    void set_Result();

};

#endif // CREATE_VIRT_DOMAIN_H
