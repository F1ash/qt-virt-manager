#include "bridge_page.h"
#include "forward_page.h"
#include <QVariant>

BridgePage::BridgePage(QWidget *parent) :
    QWizardPage(parent)
{
    br = new Bridge_Widget(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(br);
    setLayout(lt);
    connect(br, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(br, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
}
void BridgePage::initializePage()
{
    QString t, s;
    if        ( wizard()->field("BridgeType").toBool() ) {
        t = "Bridge Network";
        s =
    "The guests will be directly connected to the physical network.";
    } else if ( wizard()->field("IsolatedType").toBool() ) {
        t = "Isolated Network";
        s =
    "The guests will be directly connected to the virtual network.";
    };
    setTitle(t);
    setSubTitle(s);
}
int BridgePage::nextId() const
{
    if ( wizard()->field("IsolatedType").toBool() )
        return CreateVirtNetwork_Ass::Page_Isolated;
    ForwardPage *f = static_cast<ForwardPage*>(
                wizard()->page(CreateVirtNetwork_Ass::Page_Forward));
    if ( nullptr!=f ) {
        f->fr->setUsage(true);
        f->fr->setCurrentMode("bridge");
    };
    return CreateVirtNetwork_Ass::Page_Conclusion;
}
bool BridgePage::isComplete() const
{
    if ( wizard()->field("IsolatedType").toBool() ) {
        br->disableOptionalAttr(false);
    } else {
        br->disableOptionalAttr(true);
    };
    return ( br->isUsed() && !br->bridgeNameIsEmpty() );
}
QDomDocument BridgePage::getDataDocument() const
{
    QDomDocument doc = br->getDataDocument();
    // libvirt doesn't attempt to manage the bridge interface at all,
    // thus the <bridge> element's stp and delay attributes are not allowed
    if ( wizard()->field("BridgeType").toBool() ) {
        QDomElement _forward = doc.documentElement();
        _forward.removeAttribute("stp");
        _forward.removeAttribute("delay");
    };
    return doc;
}
