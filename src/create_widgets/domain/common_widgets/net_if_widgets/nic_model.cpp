#include "nic_model.h"

#define QEMU_DEV_MODELS QStringList()<<"ne2k_isa"\
    <<"i82551"<<"i82557b"<<"i82559er"<<"ne2k_pci"\
    <<"pcnet"<<"rtl8139"<<"e1000"<<"virtio"

NIC_Model::NIC_Model(QWidget *parent) :
    QWidget(parent)
{
    useModel = new QCheckBox("Device model:", this);
    useModel->setLayoutDirection(Qt::RightToLeft);
    model = new QComboBox(this);
    model->addItems(QEMU_DEV_MODELS);
    model->setEnabled(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(useModel, 0, 0);
    commonLayout->addWidget(model, 0, 1);
    setLayout(commonLayout);
    connect(useModel, SIGNAL(toggled(bool)),
            model, SLOT(setEnabled(bool)));
    // dataChanged connections
    connect(useModel, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}

/* public slots */
bool NIC_Model::isUsed() const
{
    return useModel->isChecked();
}
void NIC_Model::setUsage(bool state)
{
    useModel->setChecked(state);
}
QString NIC_Model::getDevModel() const
{
    if ( model->isEnabled() )
        return model->currentText();
    else return QString();
}
void NIC_Model::setDevModel(QString &_dev)
{
    int idx = model->findText(_dev, Qt::MatchContains);
    model->setCurrentIndex( (idx<0)? 0:idx );
}
