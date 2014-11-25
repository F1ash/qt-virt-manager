#include "cpu_model.h"

CPU_Model::CPU_Model(QWidget *parent) :
    QWidget(parent)
{
    doc.setContent(CPU_MODELS);
    QDomElement _el = doc
            .firstChildElement("cpus")
            .firstChildElement("arch");
    while ( !_el.isNull() ) {
        if ( _el.attribute("name")=="x86" )
            _x86 = _el;
        else if ( _el.attribute("name")=="ppc64" )
            _ppc64 = _el;
        _el = _el.nextSiblingElement("arch");
    };
    copyHostCPU = new QCheckBox("Copy Host CPU configuration", this);
    copyHostCPU->setChecked(false);
    copyHostCPU->setLayoutDirection(Qt::RightToLeft);
    model = new QComboBox(this);
    model->setEditable(true);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(copyHostCPU);
    commonLayout->addSpacing(50);
    commonLayout->addWidget(model);
    setLayout(commonLayout);
    connect(copyHostCPU, SIGNAL(toggled(bool)),
            this, SLOT(copyHostCPUState(bool)));
}

/* public slots */
void CPU_Model::archChanged(QString &_arch)
{
    //qDebug()<<_arch;
    model->clear();
    QDomElement _el, _model;
    if ( _arch=="i686" || _arch=="x86_64" )
        _el = _x86;
    else if ( _arch=="ppc64" )
        _el = _ppc64;
    _model = _el.firstChildElement("model");
    while ( !_model.isNull() ) {
        model->addItem(_model.attribute("name"));
        _model = _model.nextSiblingElement("model");
    };
    model->insertItem(0, "HyperVisor Default");
    model->insertItem(1, "Clear CPU configuration");
    model->setCurrentIndex(0);
}

/* private slots */
void CPU_Model::copyHostCPUState(bool state)
{
    model->setEnabled(!state);
}
