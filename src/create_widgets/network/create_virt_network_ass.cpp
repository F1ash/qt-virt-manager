#include "create_virt_network_ass.h"
#include <QPixmap>

CreateVirtNetwork_Ass::CreateVirtNetwork_Ass(
        QWidget *parent) :
    QWizard(parent)
{
    setWindowFlags(Qt::Widget);
    setWizardStyle(QWizard::ModernStyle);
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
    introPage = new IntroducePage(this);
    addPage(introPage);
}
