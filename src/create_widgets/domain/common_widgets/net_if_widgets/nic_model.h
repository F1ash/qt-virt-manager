#ifndef NIC_MODEL_H
#define NIC_MODEL_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>

class NIC_Model : public QWidget
{
    Q_OBJECT
public:
    explicit NIC_Model(QWidget *parent = NULL);

private:
    QCheckBox           *useModel;
    QComboBox           *model;
    QGridLayout         *commonLayout;

public slots:
    QString getDevModel() const;
};

#endif // NIC_MODEL_H
