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

signals:
    void                 dataChanged();

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
