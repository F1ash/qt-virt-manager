#include "boolean_data.h"

BOOLEAN_Data::BOOLEAN_Data(
        QWidget *parent, QString tag, QString topValue) :
    UntypedData(parent, tag)
{
    data = new QCheckBox(this);
    data->setCheckable(true);
    data->setChecked( topValue=="yes" );
    data->setToolTip("TYPE: boolean (yes/no)");
    addDataWidget(data);
    connect(data, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
}
QString BOOLEAN_Data::getAttrValue() const
{
    return (data->isChecked())? "yes" : "no";
}
void BOOLEAN_Data::setAttrValue(const QString &_value)
{
    data->setChecked( _value=="yes" );
}
void BOOLEAN_Data::clearData()
{
    data->setChecked(false);
    matchToDefault();
}
