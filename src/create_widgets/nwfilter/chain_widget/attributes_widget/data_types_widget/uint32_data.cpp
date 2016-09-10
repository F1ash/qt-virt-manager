#include "uint32_data.h"

UINT32_Data::UINT32_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QDoubleSpinBox(this);
    data->setDecimals(0);
    data->setRange(0, topValue.toDouble());
    data->setSingleStep(1);
    data->setToolTip("TYPE: UINT32");
    addDataWidget(data);
    clearData();
    connect(data, SIGNAL(valueChanged(int)),
            this, SIGNAL(dataChanged()));
}
QString UINT32_Data::getAttrValue() const
{
    return data->text();
}
void UINT32_Data::setAttrValue(const QString &_value)
{
    data->setValue(_value.toDouble());
}
void UINT32_Data::clearData()
{
    data->clear();
    matchToDefault();
}
