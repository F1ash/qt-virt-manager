#include "logical_host_cpu.h"

LogicalHostCPU::LogicalHostCPU(
        QWidget *parent, QString _caps) :
    QWidget(parent), capabilities(_caps)
{
    QDomDocument doc;
    doc.setContent(capabilities);
    //qDebug()<<capabilities;
    QDomElement _topology = doc
            .firstChildElement("capabilities")
            .firstChildElement("host")
            .firstChildElement("cpu")
            .firstChildElement("topology");
    if ( _topology.isNull() )
        cores = 1;
    else {
        int sockets = _topology.attribute("sockets").toInt();
        cores = _topology.attribute("cores").toInt()*sockets;
    };
    QString _label = QString("Logical host cores: %1").arg(cores);
    logicCPULabel = new QLabel(_label, this);
    icon = new QLabel(this);
    icon->setVisible(false);
    icon->setPixmap(QIcon::fromTheme("dialog-warning")
                    .pixmap(this->fontInfo().pixelSize()));
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
