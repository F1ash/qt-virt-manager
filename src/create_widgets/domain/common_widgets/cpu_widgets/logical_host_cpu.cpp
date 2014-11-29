#include "logical_host_cpu.h"

LogicalHostCPU::LogicalHostCPU(
        QWidget *parent, QString _caps) :
    QWidget(parent), capabilities(_caps)
{
    QDomDocument doc;
    doc.setContent(capabilities);
    //qDebug()<<capabilities;
    cores = doc.firstChildElement("capabilities")
            .firstChildElement("host")
            .firstChildElement("cpu")
            .firstChildElement("topology")
            .attribute("cores").toInt();
    cores = (cores>0)? cores : 1 ;
    QString _label = QString("Logical host CPUs: %1").arg(cores);
    logicCPULabel = new QLabel(_label, this);
    icon = new QLabel(this);
    icon->setVisible(false);
    icon->setPixmap(QIcon::fromTheme("dialog-warning")
                    .pixmap(this->font().SizeResolved));
    info = new QLabel("Overcommitting vCPUs can hurt performance", this);
    info->setVisible(false);
    commonLayout = new QHBoxLayout(this);
    commonLayout->addWidget(logicCPULabel);
    commonLayout->addWidget(icon);
    commonLayout->addWidget(info);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
}

/* public slots */
void LogicalHostCPU::changeInfoVisibility(int i)
{
    info->setVisible( i>cores );
    icon->setVisible( i>cores );
}
