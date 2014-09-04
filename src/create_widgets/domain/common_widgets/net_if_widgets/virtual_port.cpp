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
    profileIdLabel = new QLabel("ProfileId:", this);
    type = new QLineEdit(this);
    managerId = new QLineEdit(this);
    typeId = new QLineEdit(this);
    typeIdVer = new QLineEdit(this);
    instanceId = new QLineEdit(this);
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
    parametersLayout->addWidget(profileIdLabel, 5, 0);
    parametersLayout->addWidget(profileId, 5, 1);
    parameters = new QWidget(this);
    parameters->setLayout(parametersLayout);
    parameters->setVisible(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useVirtPort);
    commonLayout->addWidget(parameters);
    setLayout(commonLayout);
    connect(useVirtPort, SIGNAL(toggled(bool)),
            this, SLOT(useingChanged(bool)));
}

/* public slots */
ParameterList VirtualPort::getParameterList() const
{
    ParameterList _ret;
    if ( useVirtPort->isChecked() ) {
        if ( !type->text().isEmpty() ) {
            _ret.insert("type", type->text());
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
