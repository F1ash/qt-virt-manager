#ifndef _STORAGE_TARGET_H
#define _STORAGE_TARGET_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "create_widgets/domain/common_widgets/encryption.h"

class _Storage_Target : public QWidget
{
    Q_OBJECT
public:
    explicit _Storage_Target(
            QWidget         *parent = nullptr,
            QString          _type  = QString());
    QLineEdit       *path;
    QComboBox       *format;
    QCheckBox       *usePerm;
    QLineEdit       *owner, *group, *mode, *label;
    QWidget         *pathWdg, *formatWdg, *permissions;
    Encryption      *encrypt;

private:
    QPushButton     *pathLabel;
    QLabel          *formatLabel, *ownerLabel, *groupLabel,
                    *modeLabel, *labelLabel;
    QString          currPoolType;
    QHBoxLayout     *pathLayout;
    QGridLayout     *permLayout;
    QHBoxLayout     *formatLayout;
    QVBoxLayout     *commonLayout;

private slots:
    void             formatTypeChanged(int);
    void             setTargetDirectory();
};

#endif // _STORAGE_TARGET_H
