#include "mac_mask_data.h"

MAC_MASK_Data::MAC_MASK_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setPlaceholderText("FF:FF:FF:FC:00:00");
    data->setToolTip("TYPE: MAC_MASK");
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString MAC_MASK_Data::getAttrValue() const
{
    return data->text();
}
void MAC_MASK_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void MAC_MASK_Data::clearData()
{
    data->clear();
    matchToDefault();
}
