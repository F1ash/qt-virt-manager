#ifndef CREATE_VOLUME_H
#define CREATE_VOLUME_H

#include <QSpinBox>
#include "_create_storage.h"

class CreateVolume : public _CreateStorage
{
    Q_OBJECT
public:
    explicit CreateVolume(
            QWidget *parent = 0,
            QString _type = QString());

signals:

private:
    QString          currPoolType;

    QLabel          *formatLabel;
    QComboBox       *capLabel, *allocLabel,
                    *format;
    QSpinBox        *capacity, *allocation;
    QGridLayout     *sizeLayout, *permLayout;
    QWidget         *sizeWdg, *permissions;
    QCheckBox       *usePerm;
    QLabel          *ownerLabel, *groupLabel,
                    *modeLabel, *labelLabel;
    QLineEdit       *owner, *group, *mode, *label;

public slots:
    QString getStorageXMLDescFileName() const;

private slots:
    void formatTypeChanged(int);
};

#endif // CREATE_VOLUME_H
