#ifndef _STORAGE_TARGET_H
#define _STORAGE_TARGET_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class _Storage_Target : public QWidget
{
    Q_OBJECT
public:
    explicit _Storage_Target(
            QWidget *parent = 0,
            QString _type = QString());
    QLineEdit       *path;
    QComboBox       *format;
    QCheckBox       *usePerm;
    QLineEdit       *owner, *group, *mode, *label;
    QWidget         *pathWdg, *formatWdg, *permissions;

private:
    QLabel          *pathLabel, *formatLabel;
    QLabel          *ownerLabel, *groupLabel,
                    *modeLabel, *labelLabel;
    QString          currPoolType;
    QHBoxLayout     *pathLayout;
    QGridLayout     *permLayout;
    QHBoxLayout     *formatLayout;
    QVBoxLayout     *commonLayout;

private slots:
    void formatTypeChanged(int);
};

#endif // _STORAGE_TARGET_H
