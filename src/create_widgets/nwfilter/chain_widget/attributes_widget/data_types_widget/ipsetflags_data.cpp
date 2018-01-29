#include "ipsetflags_data.h"

IPSETFLAGS_Data::IPSETFLAGS_Data(
        QWidget *parent,
        QString  tag,
        QString  topValue) :
    UntypedData(parent, tag)
{
    data = new QLineEdit(this);
    data->setPlaceholderText(" src,src,dst,...");
    data->setToolTip(tr("TYPE: IPSETFLAGS\n\
(The source and destination flags of the ipset\n\
described by up to 6 'src' or 'dst' elements)"));
    addDataWidget(data);
    connect(data, SIGNAL(textChanged(QString)),
            this, SIGNAL(dataChanged()));
}
QString IPSETFLAGS_Data::getAttrValue() const
{
    return data->text();
}
void IPSETFLAGS_Data::setAttrValue(const QString &_value)
{
    data->setText(_value);
}
void IPSETFLAGS_Data::clearData()
{
    data->clear();
    matchToDefault();
}
