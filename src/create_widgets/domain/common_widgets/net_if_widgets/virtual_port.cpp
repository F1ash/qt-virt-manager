#include "virtual_port.h"

VirtualPort::VirtualPort(QWidget *parent) :
    _Changed(parent)
{
    useVirtPort = new QCheckBox(tr("VirtualPort"), this);
    typeLabel = new QLabel(tr("Type:"), this);
    managerIdLabel = new QLabel(tr("ManagerId:"), this);
    typeIdLabel = new QLabel(tr("TypeId:"), this);
    typeIdVerLabel = new QLabel(tr("TypeId Version:"), this);
    instanceIdLabel = new QLabel(tr("InstanceId:"), this);
    interfaceIdLabel = new QLabel(tr("InterfaceId:"), this);
    profileIdLabel = new QLabel(tr("ProfileId:"), this);
    type = new QComboBox(this);
    type->addItem("802.1Qbh", "802.1Qbh");
    type->addItem("Open vSwitch", "openvswitch");
    type->insertSeparator(2);
    type->addItem(tr("Set manually"), "");
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
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(useVirtPort, SIGNAL(toggled(bool)),
            this, SLOT(useingChanged(bool)));
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(virtPortTypeChanged(int)));
    // set empty type for the general case
    type->setCurrentIndex(type->count()-1);
    // dataChanged connects
    connect(useVirtPort, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(type, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(type, SIGNAL(editTextChanged(QString)),
            this, SLOT(stateChanged()));
    connect(managerId, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(typeId, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(typeIdVer, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(instanceId, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(interfaceId, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(profileId, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}

/* public slots */
bool VirtualPort::isUsed() const
{
    return useVirtPort->isChecked();
}
void VirtualPort::setUsage(bool state)
{
    useVirtPort->setChecked(state);
}
VirtPortParamList VirtualPort::getParameterList() const
{
    VirtPortParamList _ret;
    QString _type = type->currentData(Qt::UserRole).toString();
    if ( _type.isEmpty() ) {
        _type = type->currentData(Qt::UserRole).toString();
    };
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
void VirtualPort::setParameterList(VirtPortParamList &_list)
{
    if ( _list.keys().contains("type") ) {
        int idx = type->findData(
                    _list.value("type"),
                    Qt::UserRole,
                    Qt::MatchContains);
        idx = (idx<0)? type->count()-1:idx;
        type->setCurrentIndex( idx );
        if ( idx == type->count()-1 ) {
            type->setEditText(_list.value("type"));
        };
    };
    if ( _list.keys().contains("managerid") ) {
        managerId->setText(_list.value("managerid"));
    };
    if ( _list.keys().contains("typeid") ) {
        typeId->setText(_list.value("typeid"));
    };
    if ( _list.keys().contains("typeidver") ) {
        typeIdVer->setText(_list.value("typeidver"));
    };
    if ( _list.keys().contains("instanceid") ) {
        instanceId->setText(_list.value("instanceid"));
    };
    if ( _list.keys().contains("interfaceid") ) {
        interfaceId->setText(_list.value("interfaceid"));
    };
    if ( _list.keys().contains("profileid") ) {
        profileId->setText(_list.value("profileid"));
    };
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
