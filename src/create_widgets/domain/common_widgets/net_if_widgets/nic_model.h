#ifndef NIC_MODEL_H
#define NIC_MODEL_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include "create_widgets/domain/_changed.h"

class NIC_Model : public _Changed
{
    Q_OBJECT
public:
    explicit NIC_Model(QWidget *parent = NULL);

signals:
    //void                 dataChanged();

private:
    QCheckBox           *useModel;
    QComboBox           *model;
    QGridLayout         *commonLayout;

public slots:
    bool                 isUsed() const;
    void                 setUsage(bool);
    QString              getDevModel() const;
    void                 setDevModel(QString&);
};

#endif // NIC_MODEL_H
