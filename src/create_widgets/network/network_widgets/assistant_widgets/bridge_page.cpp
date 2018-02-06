#include "bridge_page.h"
#include "forward_page.h"
#include <QVariant>

BridgePage::BridgePage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Bridge Network"));
    setSubTitle(
tr("The guests will be directly connected to the physical network."));

    br = new Bridge_Widget(this);
    lt = new QVBoxLayout(this);
    lt->addWidget(br);
    setLayout(lt);
    connect(br, SIGNAL(toggled(bool)),
            this, SIGNAL(completeChanged()));
    connect(br, SIGNAL(dataChanged()),
            this, SIGNAL(completeChanged()));
}
int BridgePage::nextId() const
{
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
    br->disableOptionalAttr(true);
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
