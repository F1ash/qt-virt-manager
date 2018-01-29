#include "ipv6_addr_data.h"

IPv6_ADDR_Data::IPv6_ADDR_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setPlaceholderText("FFFF::1");
    data->setToolTip(tr("TYPE: IPv6_ADDR"));
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString IPv6_ADDR_Data::getAttrValue() const
{
    return data->text();
}
void IPv6_ADDR_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void IPv6_ADDR_Data::clearData()
{
    data->clear();
    matchToDefault();
}
