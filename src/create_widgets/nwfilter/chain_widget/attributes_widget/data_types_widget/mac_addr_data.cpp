#include "mac_addr_data.h"

MAC_ADDR_Data::MAC_ADDR_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setPlaceholderText("00:11:22:33:44:55");
    data->setToolTip("TYPE: MAC_ADDR");
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString MAC_ADDR_Data::getAttrValue() const
{
    return data->text();
}
void MAC_ADDR_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void MAC_ADDR_Data::clearData()
{
    data->clear();
    matchToDefault();
}
