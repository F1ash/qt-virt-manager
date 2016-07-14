#include "forward_widget.h"

#define FORWARD_MODE QStringList()\
    <<"nat"\
    <<"route"\
    <<"bridge"\
    <<"private"\
    <<"vepa"\
    <<"passthrough"\
    <<"hostdev"

Forward_Widget::Forward_Widget(
        QWidget *parent, QString tag) :
    _Checked_Widget(parent, tag)
{
    modeLabel = new QLabel("Mode:", this);
    mode = new QComboBox(this);
    mode->addItems(FORWARD_MODE);
    devLabel = new QCheckBox("Dev name:", this);
    devLabel->setToolTip(
    "if enabled, firewall rules will restrict forwarding to the named device only");
    dev = new QLineEdit(this);
    dev->setPlaceholderText("eth0 | enp2s0 | wlp3s0");
    dev->setEnabled(false);
    devLayout = new QHBoxLayout();
    devWdg = new QWidget(this);
    devWdg->setLayout(devLayout);
    frwdLayout = new QHBoxLayout();
    frwdLayout->addWidget(modeLabel);
    frwdLayout->addWidget(mode);
    devLayout->addWidget(devLabel);
    devLayout->addWidget(dev);
    frwdLayout->addWidget(devWdg);
    forwards = new QWidget(this);
    forwards->setLayout(frwdLayout);
    forwards->setEnabled(false);
    frwdModeSet = new QStackedWidget(this);
    frwdModeSet->addWidget(new NAT_Mode_widget(this));
    frwdModeSet->addWidget(new ROUTE_Mode_widget(this));
    frwdModeSet->addWidget(new BRIDGE_Mode_widget(this));
    frwdModeSet->addWidget(new PRIVATE_Mode_widget(this));
    frwdModeSet->addWidget(new VEPA_Mode_widget(this));
    frwdModeSet->addWidget(new PASSTHROUGH_Mode_widget(this));
    frwdModeSet->addWidget(new HOSTDEV_Mode_widget(this));
    frwdModeSet->setEnabled(false);
    baseLayout->addWidget(forwards);
    baseLayout->addWidget(frwdModeSet);
    baseLayout->addStretch(-1);
    connect(this, SIGNAL(toggled(bool)),
            forwards, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(toggled(bool)),
            frwdModeSet, SLOT(setEnabled(bool)));
    connect(mode, SIGNAL(currentIndexChanged(int)),
            frwdModeSet, SLOT(setCurrentIndex(int)));
    connect(mode, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(modeChanged(const QString&)));
    connect(devLabel, SIGNAL(toggled(bool)),
            dev, SLOT(setEnabled(bool)));
}

QString Forward_Widget::getCurrentMode() const
{
    return mode->currentText();
}
void Forward_Widget::setCurrentMode(const QString &_mode)
{
    // used only in Network Assistant
    // for 'bridge', 'nat', 'route' mode
    int idx = mode->findText(_mode);
    // if mode is not specified, mode='nat' is assumed
    if ( idx<0 ) idx = 0;
    mode->setCurrentIndex(idx);
    mode->setEnabled(false);
}

/* public slots */
QDomDocument Forward_Widget::getDataDocument() const
{
    QDomDocument doc, _doc;
    _QWidget *wdg = static_cast<_QWidget*>(
                frwdModeSet->currentWidget());
    if( nullptr!=wdg ) _doc = wdg->getDataDocument();
    QDomElement _forward =
            doc.createElement("forward");
    _forward.setAttribute(
                "mode",
                mode->currentText());
    if ( devLabel->isChecked() ) {
        _forward.setAttribute(
                    "dev",
                    dev->text());
    };
    if ( _doc.hasAttributes() || _doc.hasChildNodes() )
        _forward.appendChild(_doc);
    doc.appendChild(_forward);
    return doc;
}
void Forward_Widget::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    doc.setContent(_xmlDesc);
    QDomElement _network, _forward;
    _network = doc.firstChildElement("network");
    if ( !_network.isNull() ) {
        _forward = _network.firstChildElement("forward");
        if ( !_forward.isNull() ) {
            setUsage(true);
            if ( _forward.hasAttribute("dev") ) {
                devLabel->setChecked(true);
                dev->setText(_forward.attribute("dev"));
            };
            QString m = _forward.attribute("mode");
            int idx = mode->findText(m);
            // if mode is not specified, mode='nat' is assumed
            if ( idx<0 ) idx = 0;
            mode->setCurrentIndex(idx);
            QDomDocument _doc;
            _doc.setContent(QString());
            _doc.appendChild(_forward);
            QString _xml = _doc.toString();
            _QWidget *wdg = static_cast<_QWidget*>(
                        frwdModeSet->currentWidget());
            if( nullptr!=wdg ) wdg->setDataDescription(_xml);
        };
    };
}

/* private slots */
void Forward_Widget::modeChanged(const QString &_mode)
{
    bool state = ( _mode=="nat" || _mode=="route" );
    emit optionalsNeed(state);
    emit QoSAvailable(state);
    devWdg->setEnabled(state);
    if ( !state ) {
        devLabel->setChecked(false);
    };
}
