#ifndef CREATE_VOLUME_H
#define CREATE_VOLUME_H

#include <QSpinBox>
#include <QScrollArea>
#include "_create_storage_widgets/_create_storage.h"
//#include "_create_storage_widgets/_storage_source.h"
#include "_create_storage_widgets/_storage_target.h"

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

    QComboBox       *capLabel, *allocLabel;
    QSpinBox        *capacity, *allocation;
    QGridLayout     *sizeLayout;
    QWidget         *sizeWdg;
    //_Storage_Source *source;
    _Storage_Target *target;

    QScrollArea     *infoWidget;
    QVBoxLayout     *infoLayout;
    QWidget         *scrolled;

public slots:
    QString getStorageXMLDescFileName() const;
};

#endif // CREATE_VOLUME_H
