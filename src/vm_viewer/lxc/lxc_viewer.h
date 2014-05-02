#ifndef LXC_VIEWER_H
#define LXC_VIEWER_H

#include <QWidget>
#include <QPalette>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QTextStream>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include <QDebug>

class LXC_Viewer : public QWidget
{
    Q_OBJECT
public:
    explicit LXC_Viewer(QWidget *parent = 0, virConnect *conn = NULL, QString str = QString());
    ~LXC_Viewer();

signals:
    void closed();

private:
    QString              domain;
    virDomain           *domainPtr;
    virConnect          *jobConnect = NULL;
    QTextBrowser        *display;
    QVBoxLayout         *commonLayout;
    QTextStream          textStream;
    QPalette             pallete;

    virStream           *stream;

public slots:
    void close();

private slots:
    virDomain* getDomainPtr() const;

};

#endif // LXC_VIEWER_H
