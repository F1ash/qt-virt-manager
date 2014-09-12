#ifndef _CREATE_STORAGE_H
#define _CREATE_STORAGE_H

#include <QDialog>
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
#include <QDebug>

class _CreateStorage : public QDialog
{
    Q_OBJECT
public:
    explicit _CreateStorage(QWidget *parent = 0);
    QComboBox           *type;
    QLineEdit           *stName;
    QLabel              *suff;
    QVBoxLayout         *commonLayout;
    QTemporaryFile      *xml = NULL;

signals:
    void errorMsg(QString);

private:
    QLabel              *typeLabel, *stNameLabel;
    QPushButton         *chooseStorage;
    QPushButton         *cancel;
    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;

    QGridLayout         *baseLayout;
    QWidget             *baseWdg;

public slots:
    virtual QString getStorageXMLDescFileName() const;

private slots:
    void storageTypeChanged(QString);
    void set_Result();
};

#endif // _CREATE_STORAGE_H
