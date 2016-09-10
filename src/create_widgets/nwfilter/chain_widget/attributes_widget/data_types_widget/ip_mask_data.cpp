#include "ip_mask_data.h"

IP_MASK_Data::IP_MASK_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    setMatchUnusable();
    data = new QLineEdit(this);
    data->setPlaceholderText("255.255.248.0");
    data->setToolTip("TYPE: IP_MASK (dotted decimal format or CIDR)");
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString IP_MASK_Data::getAttrValue() const
{
    return data->text();
}
void IP_MASK_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void IP_MASK_Data::clearData()
{
    data->clear();
    matchToDefault();
}
