#include "_attributes.h"

_Attributes::_Attributes(QWidget *parent, QString tag) :
    _QWidget(parent)
{
    attrLabel = new QLabel(tag, this);
    attrName = new QComboBox(this);
    attrLayout = new QHBoxLayout(this);
    attrLayout->addWidget(attrLabel);
    attrLayout->addWidget(attrName);
    attrWdg = new QWidget(this);
    attrWdg->setLayout(attrLayout);
    attrEditor = new QStackedWidget(this);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(attrWdg);
    commonLayout->addWidget(attrEditor);
    setLayout(commonLayout);

    connect(attrName, SIGNAL(currentIndexChanged(int)),
            attrEditor, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(dataChanged()),
            this, SLOT(dataEdited()));
}
_Attributes::~_Attributes() {}
void _Attributes::clearAllAttributeData()
{
    for (uint i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        d->clearData();
    };
    emit released(false);
}
void _Attributes::setAttrValue(const QVariantMap &_map)
{
    int idx = attrName->findText(_map.value("name").toString());
    if ( idx>=0 ) {
        attrName->setCurrentIndex(idx);
        UntypedData *u = static_cast<UntypedData*>(
                    attrEditor->currentWidget());
        if ( u!=nullptr ) {
            // unused here
            //protocolID = _map.value("protocolID").toString();
            u->setAttrValue(_map.value("value").toString());
            if ( !u->isMatchUnusable() ) {
                QString match = _map.value("match").toString();
                bool state = (
                            match=="true" ||
                            match=="yes" ||
                            match=="1");
                u->setMatchState(state);
            };
        };
    };
}
QVariantMap _Attributes::getAttrValue(QString &attr) const
{
    QVariantMap ret;
    int idx = attrName->findText(attr);
    if ( idx>=0 ) {
        attrName->setCurrentIndex(idx);
        UntypedData *u = static_cast<UntypedData*>(
                    attrEditor->currentWidget());
        if ( u!=nullptr && !u->getAttrValue().isEmpty() ) {
            if ( !u->isMatchUnusable() && !u->isMatch() ) {
                ret.insert("match", "false");
            };
            ret.insert("name", u->getAttrName());
            ret.insert("value", u->getAttrValue());
            ret.insert("protocolID", protocolID);
        };
    };
    return ret;
}
QString _Attributes::getProtocolID() const
{
    return protocolID;
}
QStringList _Attributes::getAttrList() const
{
    QStringList l;
    for(int i=0; i<attrName->count(); i++ ) {
        l.append(attrName->itemText(i));
    };
    return l;
}

/* private slots */
void _Attributes::dataEdited()
{
    emit released(true);
}
