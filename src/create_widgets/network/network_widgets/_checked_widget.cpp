#include "_checked_widget.h"

_Checked_Widget::_Checked_Widget(
        QWidget *parent, QString _tag) :
    _QWidget(parent), tag(_tag)
{
    usage = new QCheckBox(tag, this);
    baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(3, 0, 3, 0);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
    connect(usage, SIGNAL(toggled(bool)),
            this, SIGNAL(toggled(bool)));
}

/* public slots */
bool _Checked_Widget::isUsed() const
{
    return usage->isChecked();
}
void _Checked_Widget::setUsage(bool state)
{
    usage->setChecked(state);
}
QDomDocument _Checked_Widget::getDataDocument() const
{
    return QDomDocument();
}
