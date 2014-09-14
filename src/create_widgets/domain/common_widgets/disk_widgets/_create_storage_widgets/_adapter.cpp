#include "_adapter.h"

_Adapter::_Adapter(QWidget *parent) :
    QWidget(parent)
{
    nameLabel = new QLabel("Name:", this);
    typeLabel = new QLabel("Type:", this);
    name = new QLineEdit(this);
    name->setPlaceholderText("scsi_host1");
    type = new QComboBox(this);
    connect(type, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(typeAdapterChanged(QString)));
    wwnnLabel = new QLabel("WWNN:", this);
    wwpnLabel = new QLabel("WWPN:", this);
    wwnn = new QLineEdit(this);
    wwpn = new QLineEdit(this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(new QLabel("<u>Adapter</u>", this), 0, 0);
    commonLayout->addWidget(nameLabel, 1, 0);
    commonLayout->addWidget(name, 1, 1);
    commonLayout->addWidget(typeLabel, 2, 0);
    commonLayout->addWidget(type, 2, 1);
    commonLayout->addWidget(wwnnLabel, 3, 0);
    commonLayout->addWidget(wwnn, 3, 1);
    commonLayout->addWidget(wwpnLabel, 4, 0);
    commonLayout->addWidget(wwpn, 4, 1);
    setLayout(commonLayout);
    type->addItems(QStringList()<<"scsi_host"<<"fc_host");
}

/* private slots */
void _Adapter::typeAdapterChanged(QString _type)
{
    wwnnLabel->setVisible( _type=="fc_host" );
    wwpnLabel->setVisible( _type=="fc_host" );
    wwnn->setVisible( _type=="fc_host" );
    wwpn->setVisible( _type=="fc_host" );
}
