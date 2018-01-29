#include "string_data.h"

STRING_Data::STRING_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setMaxLength(256);
    data->setPlaceholderText(tr("some string"));
    data->setToolTip(tr("TYPE: STRING (usual not more than 256 letters)"));
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString STRING_Data::getAttrValue() const
{
    return data->text();
}
void STRING_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void STRING_Data::clearData()
{
    data->clear();
    matchToDefault();
}
