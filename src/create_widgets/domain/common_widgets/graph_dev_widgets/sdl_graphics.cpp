#include "sdl_graphics.h"

SDL_Graphics::SDL_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    displayLabel = new QLabel("Display:", this);
    xauthLabel = new QLabel("Xauth:", this);
    display = new QLineEdit(this);
    display->setPlaceholderText(":0.0");
    xauth = new QLineEdit(this);
    xauth->setPlaceholderText("${HOME}/.Xauthority");
    fullscreen = new QCheckBox("Fullscreen:", this);
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
}

/* public slots */
QDomNodeList SDL_Graphics::getNodeList() const
{
    return QDomNodeList();
}
QString SDL_Graphics::getDevDisplay() const
{
    return display->text();
}
QString SDL_Graphics::getDevXauth() const
{
    return xauth->text();
}
QString SDL_Graphics::getDevFullScreen() const
{
    return (fullscreen->isChecked())? "yes" : "no";
}

/* private slots */
void SDL_Graphics::getPathToXauthority() const
{
    QString file = QFileDialog::getOpenFileName(
                NULL,
                tr("Specify Xauth"),
                "/home",
                tr("Xauth (.Xauthority)"));
    if ( !file.isEmpty() ) xauth->setText(file);
}
