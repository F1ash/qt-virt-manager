#include "boot_type.h"

BootType::BootType(QWidget *parent) :
    QWidget(parent)
{
    bootType = new QComboBox(this);
    bootType->addItem(tr("BIOS bootloader"), "bios");
    bootType->addItem(tr("Host bootloader"), "host");
    bootType->addItem(tr("Direct kernel boot"), "kernel");
    bootType->addItem(tr("Container boot"), "container");
    typeLabel = new QLabel(tr("Type:"), this);
    osType = new QLineEdit(this);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(bootType, 0, Qt::AlignLeft);
    commonLayout->addWidget(typeLabel, 0, Qt::AlignRight);
    commonLayout->addWidget(osType, 0, Qt::AlignRight);
    setLayout(commonLayout);
}
