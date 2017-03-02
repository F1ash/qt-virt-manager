#include "create_virt_network_ass.h"
#include "network_widgets/assistant_widgets/introduce_page.h"
#include "network_widgets/assistant_widgets/bridge_page.h"
#include "network_widgets/assistant_widgets/forward_page.h"
#include "network_widgets/assistant_widgets/ip_page.h"
#include "network_widgets/assistant_widgets/no_gateway_page.h"
#include "network_widgets/assistant_widgets/conclusion_page.h"
#include <QDomDocument>

CreateVirtNetwork_Ass::CreateVirtNetwork_Ass(
        QWidget *parent) :
    QWizard(parent)
{
    setWindowFlags(Qt::Widget);
    setWizardStyle(QWizard::ModernStyle);
    setOptions(
                QWizard::NoBackButtonOnStartPage |
                QWizard::NoBackButtonOnLastPage);
    setPixmap(
                QWizard::BannerPixmap,
                QPixmap());
    setPixmap(
                QWizard::LogoPixmap,
                QIcon::fromTheme("network")
                .pixmap(128));
    setPixmap(
                QWizard::WatermarkPixmap,
                QIcon::fromTheme("watermark")
                .pixmap(480));
    setPage(Page_Intro, new IntroducePage(this));
    setPage(Page_Bridge, new BridgePage(this));
    setPage(Page_Forward, new ForwardPage(this));
    setPage(Page_IP, new IP_Page(this));
    setPage(Page_NoGateway, new NoGatewayPage(this));
    setPage(Page_Conclusion, new ConclusionPage(this));

    xml = new QTemporaryFile(this);
    xml->setAutoRemove(false);
    xml->setFileTemplate(
                QString("%1%2XML_Desc-XXXXXX.xml")
                .arg(QDir::tempPath())
                .arg(QDir::separator()));
    connect(this, SIGNAL(finished(int)),
            this, SLOT(set_Result(int)));
}
QString CreateVirtNetwork_Ass::getXMLDescFileName() const
{
    return xml->fileName();
}

/* private slots */
int CreateVirtNetwork_Ass::nextId() const
{
    switch (currentId()) {
    case Page_Intro:
    case Page_Bridge:
    case Page_Forward:
    case Page_IP:
    case Page_NoGateway:
        return currentPage()->nextId();
    default:
        return -1;
    }
}
void CreateVirtNetwork_Ass::buildXMLDescription()
{
    QDomDocument doc;
    //qDebug()<<doc.toString();
    QDomElement _xmlDesc, _name;
    QDomText data;

    _xmlDesc = doc.createElement("network");
    if ( field("NoGatewayIPv6").toBool() ) {
        _xmlDesc.setAttribute("ipv6", "yes");
    };
    _name = doc.createElement("name");
    data = doc.createTextNode(
                field("NetworkName").toString());
    _name.appendChild(data);
    _xmlDesc.appendChild(_name);

    BridgePage *b = static_cast<BridgePage*>(page(Page_Bridge));
    if ( nullptr!=b && b->br->isUsed() ) {
        _xmlDesc.appendChild(
                    b->getDataDocument());
    };
    ForwardPage *f = static_cast<ForwardPage*>(page(Page_Forward));
    if ( nullptr!=f  && f->fr->isUsed() ) {
        _xmlDesc.appendChild(
                    f->getDataDocument());
    };
    IP_Page *i = static_cast<IP_Page*>(page(Page_IP));
    if ( nullptr!=i && i->isUsed() ) {
        _xmlDesc.appendChild(
                    i->getDataDocument());
    };
    doc.appendChild(_xmlDesc);

    bool read = xml->open();
    if (read) xml->write(doc.toByteArray(4).data());
    xml->close();
}
void CreateVirtNetwork_Ass::set_Result(int res)
{
    this->setEnabled(false);
    if ( res==QDialog::Accepted ) {
        buildXMLDescription();
        emit accepted(true);
    } else {
        emit accepted(false);
    };
}
