#include "_disk_item.h"

_DiskItem::_DiskItem(QWidget *parent) : QWidget(parent)
{
    usage = new QCheckBox(this);
    baseLayout = new QHBoxLayout(this);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setDisabled(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(usage);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(usage, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setEnabled(bool)));
}

_DiskItem::~_DiskItem()
{

}

