#include "untyped_data.h"

UntypedData::UntypedData(QWidget *parent, QString tag) :
    QWidget(parent)
{
    match = new QCheckBox(this);
    match->setCheckable(true);
    match->setChecked(true);    // default
    match->setToolTip("Match");
    name = new QLabel(tag, this);
    clear = new QPushButton(
                QIcon::fromTheme("edit-clear"),
                "",
                this);
    clear->setToolTip("Clear attribute data");
    connect(clear, SIGNAL(clicked(bool)),
            this, SLOT(clearData()));
    connect(match, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));

    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(match, 1);
    commonLayout->addWidget(name, 5);
    commonLayout->addWidget(clear, 1);
    setLayout(commonLayout);
}
void UntypedData::setMatchUnusable()
{
    match->setChecked(false);
    match->setDisabled(true);
}
bool UntypedData::isMatchUnusable() const
{
    return !match->isEnabled();
}
bool UntypedData::isMatch() const
{
    return match->isChecked();
}
void UntypedData::matchToDefault()
{
    match->setChecked(true);    // default
}
void UntypedData::setMatchState(bool state)
{
    match->setChecked(state);
}
QString UntypedData::getAttrName() const
{
    return name->text();
}
QString UntypedData::getAttrValue() const
{
    return "";
}
void UntypedData::setAttrValue(const QString &_value)
{
    Q_UNUSED(_value)
}
void UntypedData::addDataWidget(QWidget *w)
{
    commonLayout->insertWidget(2, w, 5);
}

void UntypedData::clearData()
{
    matchToDefault();
}
