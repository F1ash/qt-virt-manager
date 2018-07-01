#include "_adapter.h"

_Adapter::_Adapter(QWidget *parent) :
    QWidget(parent)
{
    nameLabel = new QLabel(tr("Name:"), this);
    typeLabel = new QLabel(tr("Type:"), this);
    name = new QLineEdit(this);
    name->setPlaceholderText("scsi_host1");
    type = new QComboBox(this);
    connect(type, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(typeAdapterChanged(QString)));
    wwnnLabel = new QLabel("WWNN:", this);
    wwpnLabel = new QLabel("WWPN:", this);
    adapterPrntLabel = new QLabel(tr("Parent"), this);
    wwnn = new QLineEdit(this);
    wwnn->setPlaceholderText("20000000c9831b4b");
    wwpn = new QLineEdit(this);
    wwpn->setPlaceholderText("10000000c9831b4b");
    adapterPrnt = new QLineEdit(this);
    adapterPrnt->setPlaceholderText("scsi_host5");
    usePrntAddr = new QCheckBox(tr("Use Parent Address"), this);
    adapterPrntIDLabel = new QLabel(tr("Unique_ID:"), this);
    adapterPrntIDLabel->setVisible(false);
    adapterPrntID = new QLineEdit(this);
    adapterPrntID->setPlaceholderText("1");
    adapterPrntID->setVisible(false);
    addr = new PciAddr(this);
    addr->setVisible(false);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(new QLabel(tr("<u>Adapter</u>"), this), 0, 0);
    commonLayout->addWidget(nameLabel, 1, 0);
    commonLayout->addWidget(name, 1, 1);
    commonLayout->addWidget(typeLabel, 2, 0);
    commonLayout->addWidget(type, 2, 1);
    commonLayout->addWidget(wwnnLabel, 3, 0);
    commonLayout->addWidget(wwnn, 3, 1);
    commonLayout->addWidget(wwpnLabel, 4, 0);
    commonLayout->addWidget(wwpn, 4, 1);
    commonLayout->addWidget(adapterPrntLabel, 5, 0);
    commonLayout->addWidget(adapterPrnt, 5, 1);
    commonLayout->addWidget(usePrntAddr, 6, 0);
    commonLayout->addWidget(adapterPrntIDLabel, 7, 0);
    commonLayout->addWidget(adapterPrntID, 7, 1);
    commonLayout->addWidget(addr, 8, 0, 9, 2);
    setLayout(commonLayout);
    connect(usePrntAddr, SIGNAL(toggled(bool)),
            this, SLOT(usingPrntAddrChanged(bool)));
    type->addItems(QStringList()<<"scsi_host"<<"fc_host");
}

/* private slots */
void _Adapter::typeAdapterChanged(QString _type)
{
    wwnnLabel->setVisible( _type.compare("fc_host")==0 );
    wwpnLabel->setVisible( _type.compare("fc_host")==0 );
    wwnn->setVisible( _type.compare("fc_host")==0 );
    wwpn->setVisible( _type.compare("fc_host")==0 );
    adapterPrntLabel->setVisible( _type.compare("fc_host")==0 );
    adapterPrnt->setVisible( _type.compare("fc_host")==0 );
    usePrntAddr->setVisible( _type.compare("scsi_host")==0 );
    if ( _type.compare("fc_host")==0 ) usePrntAddr->setChecked( false );
}

/* private slots */
void _Adapter::usingPrntAddrChanged(bool state)
{
    adapterPrntIDLabel->setVisible(state);
    adapterPrntID->setVisible(state);
    addr->setVisible(state);
    nameLabel->setEnabled(!state);
    name->setEnabled(!state);
    typeLabel->setEnabled(!state);
    type->setEnabled(!state);
}
