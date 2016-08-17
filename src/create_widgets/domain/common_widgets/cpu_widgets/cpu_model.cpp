#include "cpu_model.h"

CPU_Model::CPU_Model(QWidget *parent) :
    _Changed(parent)
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
    useModel = new QCheckBox("Use model", this);
    copyHostCPU = new QCheckBox("Exactly the same as the host CPU", this);
    copyHostCPU->setChecked(false);
    model = new QComboBox(this);
    model->setEditable(false);
    model->setEditText("");
    matchLabel = new QLabel("Match:", this);
    match = new QComboBox(this);
    match->addItems(QStringList()<<"exact"<<"strict"<<"minimum");
    match->setEnabled(false);
    allowFallback = new QCheckBox("Allow fallback", this);
    baseLayout = new QGridLayout();
    baseLayout->addWidget(copyHostCPU, 0, 0);
    baseLayout->addWidget(model, 0, 1);
    baseLayout->addWidget(allowFallback, 1, 0);
    baseLayout->addWidget(matchLabel, 1, 1, Qt::AlignLeft);
    baseLayout->addWidget(match, 1, 1, Qt::AlignRight);
    baseWdg = new QWidget(this);
    baseWdg->setLayout(baseLayout);
    baseWdg->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(useModel);
    commonLayout->addWidget(baseWdg);
    setLayout(commonLayout);
    connect(useModel, SIGNAL(toggled(bool)),
            baseWdg, SLOT(setEnabled(bool)));
    connect(copyHostCPU, SIGNAL(toggled(bool)),
            this, SLOT(copyHostCPUState(bool)));
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(modelChanged(int)));
    // dataChanged connections
    connect(useModel, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(allowFallback, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(copyHostCPU, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
    connect(model, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
    connect(match, SIGNAL(currentIndexChanged(int)),
            this, SLOT(stateChanged()));
}

/* public slots */
bool CPU_Model::isUsed() const
{
    return useModel->isChecked();
}
void CPU_Model::setUsage(bool state)
{
    useModel->setChecked(state);
}
void CPU_Model::archChanged(const QString &_arch)
{
    //qDebug()<<_arch;
    model->clear();
    QDomElement _el, _model;
    if ( _arch=="i686" || _arch=="x86_64" )
        _el = _x86;
    else if ( _arch=="ppc64" || _arch=="ppc" )
        _el = _ppc64;
    _model = _el.firstChildElement("model");
    while ( !_model.isNull() ) {
        model->addItem(_model.attribute("name"), "custom");
        _model = _model.nextSiblingElement("model");
    };
    model->insertItem(0, "Copy Host CPU configuration", "host-model");
    model->insertItem(1, "Manually set guest CPU", "custom");
    model->setCurrentIndex(0);
    setModel(currModel);
    // insert the separator after set the Model, because
    // the Model value can be the empty
    model->insertSeparator(2);
}
QString CPU_Model::getModel() const
{
    QString _res;
    if ( model->currentIndex()>0 ) {
        _res = model->currentText();
    };
    return _res;
}
QString CPU_Model::getMode() const
{
    return model->itemData(
                model->currentIndex(),
                Qt::UserRole).toString();
}
QString CPU_Model::getMatch() const
{
    QString _res, _mode;
    _mode = model->itemData(
                model->currentIndex(), Qt::UserRole).toString();
    if ( _mode!="host-model" ) _res = match->currentText();
    return _res;
}
void CPU_Model::setModel(QString &_model)
{
    int idx;
    currModel = _model;
    if ( _model.isEmpty() ) {
        idx = 0;
        model->setCurrentIndex(idx);
        return;
    };
    idx = model->findText(_model, Qt::MatchExactly);
    if ( idx<0 ) {
        idx = 1;
        model->setCurrentIndex(idx);
        model->setEditText(_model);
    } else
        model->setCurrentIndex(idx);
}
void CPU_Model::setMatch(QString &_match)
{
    int idx = match->findText(_match, Qt::MatchContains);
    match->setCurrentIndex( (idx<0)? 0:idx );
}

/* private slots */
void CPU_Model::copyHostCPUState(bool state)
{
    model->setEnabled( !state );
    if ( state ) {
        match->setEnabled(false);
    } else {
        int idx = model->currentIndex();
        QString _mode = model->itemData(idx, Qt::UserRole).toString();
        match->setEnabled( _mode!="host-model" );
    };
    matchLabel->setEnabled( !state );
    allowFallback->setEnabled( !state );
    emit cpuUseElements(!state);
}
void CPU_Model::modelChanged(int i)
{
    model->setEditable(i==1);
    //if (i==1) model->clearEditText();
    QString _mode = model->itemData(i, Qt::UserRole).toString();
    match->setEnabled( _mode!="host-model" );
}
