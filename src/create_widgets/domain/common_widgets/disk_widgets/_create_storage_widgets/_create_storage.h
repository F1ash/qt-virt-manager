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
    QTemporaryFile      *xml;
    QStackedWidget      *info;
    QScrollArea         *infoWidget;

    QSettings            settings;
    QString              storageType;
    QCheckBox           *showAtClose;

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

    QVBoxLayout         *infoLayout;
    QWidget             *scrolled;

public slots:
    virtual QString getStorageXMLDescFileName() const;
            bool    showXMLDescription() const;

private slots:
    void set_Result();
};

#endif // _CREATE_STORAGE_H
