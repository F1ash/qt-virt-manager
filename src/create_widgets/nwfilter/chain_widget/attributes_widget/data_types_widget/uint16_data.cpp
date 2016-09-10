#include "uint16_data.h"

UINT16_Data::UINT16_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QSpinBox(this);
    data->setRange(0, topValue.toInt());
    data->setToolTip("TYPE: UINT16");
    addDataWidget(data);
    clearData();
    connect(data, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}
QString UINT16_Data::getAttrValue() const
{
    return data->text();
}
void UINT16_Data::setAttrValue(const QString &_value)
{
    data->setValue(_value.toInt());
}
void UINT16_Data::clearData()
{
    data->clear();
    matchToDefault();
}
