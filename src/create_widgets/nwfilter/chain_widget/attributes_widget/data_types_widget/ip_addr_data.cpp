#include "ip_addr_data.h"

IP_ADDR_Data::IP_ADDR_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setPlaceholderText("10.1.2.3");
    data->setToolTip("TYPE: IP_ADDR");
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString IP_ADDR_Data::getAttrValue() const
{
    return data->text();
}
void IP_ADDR_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void IP_ADDR_Data::clearData()
{
    data->clear();
    matchToDefault();
}
