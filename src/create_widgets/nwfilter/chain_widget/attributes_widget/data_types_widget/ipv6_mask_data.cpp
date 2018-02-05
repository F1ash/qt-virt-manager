#include "ipv6_mask_data.h"

IPv6_MASK_Data::IPv6_MASK_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    setMatchUnusable();
    data = new QLineEdit(this);
    data->setPlaceholderText("FFFF:FFFF:FC00::");
    data->setToolTip(tr("TYPE: IPv6_MASK (numbers format or CIDR)"));
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString IPv6_MASK_Data::getAttrValue() const
{
    return data->text();
}
void IPv6_MASK_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void IPv6_MASK_Data::clearData()
{
    data->clear();
    matchToDefault();
}
