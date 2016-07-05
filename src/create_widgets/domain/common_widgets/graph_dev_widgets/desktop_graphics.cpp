#include "desktop_graphics.h"

Desktop_Graphics::Desktop_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    displayLabel = new QLabel("Display:", this);
    display = new QLineEdit(this);
    display->setPlaceholderText(":0.0");
    fullscreen = new QCheckBox("Fullscreen", this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(displayLabel, 0, 0);
    commonLayout->addWidget(display, 0, 1);
    commonLayout->addWidget(fullscreen, 1, 1, Qt::AlignTop);
    setLayout(commonLayout);
    // dataChanged connections
    connect(display, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(fullscreen, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument Desktop_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "desktop");
    _devDesc.setAttribute("display", display->text());
    _devDesc.setAttribute("fullscreen", (fullscreen->isChecked())? "yes" : "no");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    return doc;
}
void Desktop_Graphics::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    QDomElement _device;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    display->setText(
                _device.attribute("display"));
    fullscreen->setChecked(
                _device.attribute("fullscreen")=="yes");
}
