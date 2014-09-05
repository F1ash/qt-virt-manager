#include "virtual_port.h"

VirtualPort::VirtualPort(QWidget *parent) :
    QWidget(parent)
{
    useVirtPort = new QCheckBox("VirtualPort", this);
    typeLabel = new QLabel("Type:", this);
    managerIdLabel = new QLabel("ManagerId:", this);
    typeIdLabel = new QLabel("TypeId:", this);
    typeIdVerLabel = new QLabel("TypeId Version:", this);
    instanceIdLabel = new QLabel("InstanceId:", this);
    interfaceIdLabel = new QLabel("InterfaceId:", this);
    profileIdLabel = new QLabel("ProfileId:", this);
    type = new QComboBox(this);
    type->addItem("802.1Qbh", "802.1Qbh");
    type->addItem("Open vSwitch", "openvswitch");
    type->insertSeparator(2);
    type->addItem("Set manually", "");
    type->setEditText("");
    managerId = new QLineEdit(this);
    typeId = new QLineEdit(this);
    typeIdVer = new QLineEdit(this);
    instanceId = new QLineEdit(this);
    interfaceId = new QLineEdit(this);
    profileId = new QLineEdit(this);
    parametersLayout = new QGridLayout();
    parametersLayout->addWidget(typeLabel, 0, 0);
    parametersLayout->addWidget(type, 0, 1);
    parametersLayout->addWidget(managerIdLabel, 1, 0);
    parametersLayout->addWidget(managerId, 1, 1);
    parametersLayout->addWidget(typeIdLabel, 2, 0);
    parametersLayout->addWidget(typeId, 2, 1);
    parametersLayout->addWidget(typeIdVerLabel, 3, 0);
    parametersLayout->addWidget(typeIdVer, 3, 1);
    parametersLayout->addWidget(instanceIdLabel, 4, 0);
    parametersLayout->addWidget(instanceId, 4, 1);
    parametersLayout->addWidget(interfaceIdLabel, 5, 0);
    parametersLayout->addWidget(interfaceId, 5, 1);
    parametersLayout->addWidget(profileIdLabel, 6, 0);
    parametersLayout->addWidget(profileId, 6, 1);
    parameters = new QWidget(this);
    parameters->setLayout(parametersLayout);
    parameters->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useVirtPort);
    commonLayout->addWidget(parameters);
    setLayout(commonLayout);
    connect(useVirtPort, SIGNAL(toggled(bool)),
            this, SLOT(useingChanged(bool)));
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(virtPortTypeChanged(int)));
    // set empty type for the general case
    type->setCurrentIndex(type->count()-1);
}

/* public slots */
ParameterList VirtualPort::getParameterList() const
{
    ParameterList _ret;
    QString _type = type->itemData(type->currentIndex(), Qt::UserRole).toString();
    if ( _type.isEmpty() ) _type = type->currentText();
    if ( useVirtPort->isChecked() ) {
        if ( !_type.isEmpty() ) {
            _ret.insert("type", _type);
        };
        if ( !managerId->text().isEmpty() ) {
            _ret.insert("managerid", managerId->text());
        };
        if ( !typeId->text().isEmpty() ) {
            _ret.insert("typeid", typeId->text());
        };
        if ( !typeIdVer->text().isEmpty() ) {
            _ret.insert("typeidversion", typeIdVer->text());
        };
        if ( !instanceId->text().isEmpty() ) {
            _ret.insert("instanceid", instanceId->text());
        };
        if ( !interfaceId->text().isEmpty() ) {
            _ret.insert("interfaceid", interfaceId->text());
        };
        if ( !profileId->text().isEmpty() ) {
            _ret.insert("profileid", profileId->text());
        };
    };
    return _ret;
}

/* private slots */
void VirtualPort::useingChanged(bool state)
{
    parameters->setVisible(state);
}
void VirtualPort::virtPortTypeChanged(int i)
{
    QString _type = type->itemData(i, Qt::UserRole).toString();
    if ( _type=="802.1Qbh" ) {
        type->setEditable(false);
        managerId->setVisible(true);
        typeId->setVisible(true);
        typeIdVer->setVisible(true);
        instanceId->setVisible(true);
        interfaceId->setVisible(false);
    } else if ( _type=="openvswitch" ) {
        type->setEditable(false);
        managerId->setVisible(false);
        typeId->setVisible(false);
        typeIdVer->setVisible(false);
        instanceId->setVisible(false);
        interfaceId->setVisible(true);
    } else {
        type->setEditable(true);
        managerId->setVisible(true);
        typeId->setVisible(true);
        typeIdVer->setVisible(true);
        instanceId->setVisible(true);
        interfaceId->setVisible(true);
    };
    // clear data for new virtPort type
    managerId->clear();
    typeId->clear();
    typeIdVer->clear();
    instanceId->clear();
    interfaceId->clear();
    type->clearEditText();
}
