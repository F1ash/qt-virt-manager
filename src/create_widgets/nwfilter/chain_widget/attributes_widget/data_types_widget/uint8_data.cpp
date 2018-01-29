#include "uint8_data.h"

UINT8_Data::UINT8_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    data = new QSpinBox(this);
    data->setRange(0, topValue.toInt());
    data->setToolTip(tr("TYPE: UINT8"));
    addDataWidget(data);
    clearData();
    connect(data, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}
QString UINT8_Data::getAttrValue() const
{
    return data->text();
}
void UINT8_Data::setAttrValue(const QString &_value)
{
    data->setValue(_value.toInt());
}
void UINT8_Data::clearData()
{
    data->clear();
    matchToDefault();
}
