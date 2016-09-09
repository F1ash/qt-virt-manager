#include "untyped_data.h"

UntypedData::UntypedData(QWidget *parent, QString tag) :
    QWidget(parent)
{
    match = new QCheckBox(this);
    match->setCheckable(true);
    match->setChecked(true);
    match->setToolTip("Match");
    name = new QLabel(tag, this);
    clear = new QPushButton(
                QIcon::fromTheme("edit-clear"),
                "",
                this);
    connect(clear, SIGNAL(clicked(bool)),
            this, SLOT(clearData()));

    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(match);
    commonLayout->addWidget(name);
    commonLayout->addWidget(clear);
    setLayout(commonLayout);
}
bool UntypedData::isMatch() const
{
    return match->isChecked();
}
QString UntypedData::getAttrName() const
{
    return name->text();
}
QString UntypedData::getAttrValue() const
{
    return "";
}
void UntypedData::addDataWidget(QWidget *w)
{
    commonLayout->insertWidget(2, w);
}

void UntypedData::clearData()
{

}
