#include "nwfilter_parameters.h"

#define DESERVED_VARs QStringList()\
<<"MAC"\
<<"IP"\
<<"IP6"\
<<"DHCPSERVER"\
<<"DHCPSERVERV6"

NWFilter_Params::NWFilter_Params(
        QWidget *parent, QString tag) :
    _List_Widget(parent, tag)
{
    list->setToolTip(
    "Network filter parameters:\nVARIABLE = VALUE");
    nwFilters = new QComboBox(this);
    nwFilters->setToolTip("Availabled network filters");
    useAutoIP = new QCheckBox("Autodetect IP", this);
    autoIpTypes = new QComboBox(this);
    autoIpTypes->addItems(QStringList()<<"any"<<"dhcp"<<"none");
    autoIpTypes->setEnabled(false);
    autoIpLayout = new QHBoxLayout();
    autoIpLayout->addWidget(useAutoIP);
    autoIpLayout->addWidget(autoIpTypes);
    autoIP = new QWidget(this);
    autoIP->setLayout(autoIpLayout);
    baseLayout->insertWidget(0, nwFilters);
    baseLayout->insertWidget(1, autoIP);
    paramName = new QComboBox(this);
    paramName->setEditable(true);
    paramName->insertItem(0, "");
    paramName->addItems(DESERVED_VARs);
    paramVal = new QLineEdit(this);
    paramVal->setPlaceholderText("VARIABLE's VALUE");
    panelLayout->insertWidget(1, paramName);
    panelLayout->insertWidget(2, paramVal);
    connect(useAutoIP, SIGNAL(toggled(bool)),
            autoIpTypes, SLOT(setEnabled(bool)));
    connect(paramName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(variableChanged(int)));
    // dataChanged connections
    connect(useAutoIP, SIGNAL(toggled(bool)),
            this, SIGNAL(dataChanged()));
    connect(nwFilters, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
    connect(autoIpTypes, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(dataChanged()));
}
void NWFilter_Params::setNWFiltersList(const QStringList &l)
{
    nwFilters->addItems(l);
}

/* public slots */
void NWFilter_Params::addItem()
{
    QString p, v;
    p = paramName->currentText();
    v = paramVal->text();
    if ( !p.isEmpty() && !v.isEmpty() ) {
        list->addItem(QString("%1=%2").arg(p).arg(v));
        paramVal->clear();
        paramName->clearEditText();
    };
}
QDomDocument NWFilter_Params::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _filterref;
    _filterref = doc.createElement("filterref");
    _filterref.setAttribute("filter", nwFilters->currentText());
    for ( uint i=0; i<list->count(); i++ ) {
        QListWidgetItem *item = list->item(i);
        if ( item==nullptr ) continue;
        QStringList _expr = item->text().split("=");
        QDomElement _parameter = doc.createElement("parameter");
        _parameter.setAttribute("name", _expr.first());
        _parameter.setAttribute("value", _expr.last());
        _filterref.appendChild(_parameter);
    };
    if ( useAutoIP->isChecked() ) {
        QDomElement _parameter = doc.createElement("parameter");
        _parameter.setAttribute(
                    "name", "CTRL_IP_LEARNING");
        _parameter.setAttribute(
                    "value", autoIpTypes->currentText());
        _filterref.appendChild(_parameter);
    };
    doc.appendChild(_filterref);
    return doc;
}
void NWFilter_Params::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _filterref = doc.documentElement();
    if ( !_filterref.isNull() ) {
        QString f = _filterref.attribute("filter");
        int idx = nwFilters->findText(f);
        if (-1<idx) {
            setUsage(true);
            nwFilters->setCurrentIndex(idx);
        } else {
            setUsage(false);
        };
        QDomElement _parameter =
                _filterref.firstChildElement("parameter");
        while (!_parameter.isNull()) {
            QString p, v;
            p = _parameter.attribute("name");
            v = _parameter.attribute("value");
            if ( p=="CTRL_IP_LEARNING" ) {
                useAutoIP->setChecked(true);
                idx = autoIpTypes->findText(v);
                if (-1==idx) idx = 0;
                autoIpTypes->setCurrentIndex(idx);
            } else {
                list->addItem(QString("%1=%2").arg(p).arg(v));
            };
            _parameter = _parameter.nextSiblingElement("parameter");
        };
    };
}

/* private slots */
void NWFilter_Params::variableChanged(int i)
{
    paramVal->clear();
    paramName->clearEditText();
    if ( i==0 ) {
        paramName->setEditable(true);
    } else {
        paramName->setEditable(false);
    };
}
