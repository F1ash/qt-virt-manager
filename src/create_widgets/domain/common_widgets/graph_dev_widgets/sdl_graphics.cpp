#include "sdl_graphics.h"

SDL_Graphics::SDL_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    displayLabel = new QLabel(tr("Display:"), this);
    xauthLabel = new QLabel(tr("Xauth:"), this);
    display = new QLineEdit(this);
    display->setPlaceholderText(":0.0");
    xauth = new QLineEdit(this);
    xauth->setPlaceholderText("${HOME}/.Xauthority");
    fullscreen = new QCheckBox(tr("Fullscreen"), this);
    browse = new QPushButton(QIcon::fromTheme("edit-find"), "", this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(displayLabel, 0, 0);
    commonLayout->addWidget(display, 0, 1);
    commonLayout->addWidget(xauthLabel, 1, 0);
    commonLayout->addWidget(xauth, 1, 1);
    commonLayout->addWidget(browse, 1, 2);
    commonLayout->addWidget(fullscreen, 2, 1, Qt::AlignTop);
    setLayout(commonLayout);
    connect(browse, SIGNAL(clicked()),
            this, SLOT(getPathToXauthority()));
    // dataChanged connections
    connect(display, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
    connect(xauth, SIGNAL(textChanged(QString)),
            this, SLOT(stateChanged()));
    connect(fullscreen, SIGNAL(toggled(bool)),
            this, SLOT(stateChanged()));
}

/* public slots */
QDomDocument SDL_Graphics::getDataDocument() const
{
    QDomDocument doc;
    QDomElement _device, _devDesc;
    _device = doc.createElement("device");
    _devDesc = doc.createElement("graphics");
    _devDesc.setAttribute("type", "sdl");
    _devDesc.setAttribute("display", display->text());
    _devDesc.setAttribute("xauth", xauth->text());
    _devDesc.setAttribute(
                "fullscreen",
                (fullscreen->isChecked())? "yes" : "no");
    _device.appendChild(_devDesc);
    doc.appendChild(_device);
    //qDebug()<<doc.toString();
    return doc;
}
void SDL_Graphics::setDataDescription(const QString &_xmlDesc)
{
    QDomDocument doc;
    QDomElement _device;
    doc.setContent(_xmlDesc);
    _device = doc.firstChildElement("device")
            .firstChildElement("graphics");
    display->setText(
                _device.attribute("display"));
    xauth->setText(
                _device.attribute("xauth"));
    fullscreen->setChecked(
                _device.attribute("fullscreen").compare("yes")==0);
}

/* private slots */
void SDL_Graphics::getPathToXauthority() const
{
    QString file = QFileDialog::getOpenFileName(
                nullptr,
                tr("Specify Xauth"),
                "/home",
                "Xauth (.Xauthority)");
    if ( !file.isEmpty() ) xauth->setText(file);
}
