#ifndef _TARGET_H
#define _TARGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QMap>
#include <QString>

typedef QMap<QString, QString> TargetAttrs;

class _Target : public QWidget
{
    Q_OBJECT
public:
    explicit _Target(QWidget *parent = NULL);

private:
    QLabel      *devNameLabel, *busLabel;
    QCheckBox   *trayLabel, *removableLabel;
    QLineEdit   *devName;
    QComboBox   *bus, *tray, *removable;
    QGridLayout *commonLayout;

public slots:
    TargetAttrs getTargetAttrs() const;
};

#endif // _TARGET_H
