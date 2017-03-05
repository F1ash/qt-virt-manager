#include "encryption.h"

Encryption::Encryption(QWidget *parent) :
    QWidget(parent)
{
    useEncryption = new QCheckBox("Use Encryption", this);
    formatLabel = new QLabel("Format:", this);
    format = new QComboBox(this);
    format->addItems(QStringList()<<"default"<<"qcow"<<"luks");
    baseLayout = new QGridLayout();
    baseLayout->addWidget(formatLabel, 0, 0);
    baseLayout->addWidget(format, 0, 1);
    baseLayout->addWidget(
                new QLabel("The Secret will be auto-generate.", this),
                1, 0, 2, 2);
    // TODO: implement tag 'secret'
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useEncryption);
    commonLayout->addWidget(baseWdg);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(useEncryption, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setVisible(bool)));
}

bool Encryption::isUsed() const
{
    return useEncryption->isChecked();
}
void Encryption::setUsage(const bool state)
{
    useEncryption->setChecked(state);
}
QString Encryption::getFormat() const
{
    return format->currentText().toLower();
}
void Encryption::setFormat(const QString &_fmt)
{
    int idx = format->findText(_fmt);
    if ( idx<0 ) idx = 0;
    format->setCurrentIndex(idx);
}
