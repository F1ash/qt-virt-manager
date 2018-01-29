#include "boolean_data.h"

BOOLEAN_Data::BOOLEAN_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    changed = false;
    data = new QCheckBox(this);
    data->setCheckable(true);
    bool state = (
                topValue=="true" ||
                topValue=="yes" ||
                topValue=="1");
    data->setChecked( state );
    data->setToolTip(tr("TYPE: boolean (yes/no)"));
    addDataWidget(data);
    connect(data, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(this, SIGNAL(dataChanged()),
            this, SLOT(dataWasChanged()));
}
QString BOOLEAN_Data::getAttrValue() const
{
    QString ret;
    if ( changed ) {
        ret = (data->isChecked())? "true" : "false";
    };
    return ret;
}
void BOOLEAN_Data::setAttrValue(const QString &_value)
{
    data->setChecked(
                _value=="true" ||
                _value=="yes" ||
                _value=="1");
}
void BOOLEAN_Data::dataWasChanged()
{
    changed = true;
}
void BOOLEAN_Data::clearData()
{
    changed = false;
    data->setChecked(false);
    matchToDefault();
}
