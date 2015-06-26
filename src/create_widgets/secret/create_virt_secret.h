#ifndef CREATE_VIRT_SECRET_H
#define CREATE_VIRT_SECRET_H

#include <QDialog>
#include <QSettings>
#include <QScrollArea>
#include <QPushButton>
#include <QDir>
#include <QTemporaryFile>
#include <QLabel>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QDomDocument>
#include <QDebug>

class CreateVirtSecret : public QDialog
{
    Q_OBJECT
public:
    explicit CreateVirtSecret(QWidget *parent = NULL);
    ~CreateVirtSecret();

signals:
    void             errorMsg(QString);

private:
    QSettings        settings;
    QScrollArea     *scroll;
    QWidget         *scrolled;
    QVBoxLayout     *scrollLayout;
    QWidget         *baseWdg;
    QGridLayout     *baseLayout;

    QCheckBox       *showDescription;
    QLabel          *about;
    QPushButton     *ok;
    QPushButton     *cancel;
    QHBoxLayout     *buttonLayout;
    QWidget         *buttons;

    QVBoxLayout     *secDescLayout;

    QTemporaryFile  *xml;

public slots:
    QString          getXMLDescFileName() const;
    bool             getShowing() const;

private slots:
    void             buildXMLDescription();
    void             set_Result();
    void             secretTypeChanged(bool);
};

#endif // CREATE_VIRT_SECRET_H
