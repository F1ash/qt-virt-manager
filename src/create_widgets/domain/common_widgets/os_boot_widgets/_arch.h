#ifndef _ARCH_H
#define _ARCH_H

#include "create_widgets/domain/_qwidget.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QDomDocument>

class _Arch : public _QWidget
{
    Q_OBJECT
public:
    explicit _Arch(QWidget *parent = 0,
                   QString _caps = "");

signals:
    void             domainType(QString&);
    void             archType(QString&);
    void             osType(QString&);
    void             emulatorType(QString&);

private:
    QString          capabilities;
    QDomDocument     doc;
    QLabel          *archLabel, *machLabel;
    QComboBox       *arch, *machines;
    QGridLayout     *commonLayout;

public slots:
    void             setItems();
    void             setArch(const QString&);
    void             setMachine(const QString&);
    QString          getMachine() const;

private slots:
    void             archChanged(const QString&);
};

#endif // _ARCH_H
