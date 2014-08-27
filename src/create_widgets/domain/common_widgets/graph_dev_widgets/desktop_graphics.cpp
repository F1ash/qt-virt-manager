#include "desktop_graphics.h"

Desktop_Graphics::Desktop_Graphics(QWidget *parent) :
    _QWidget(parent)
{
    displayLabel = new QLabel("Display:", this);
    display = new QLineEdit(this);
    display->setPlaceholderText(":0.0");
    fullscreen = new QCheckBox("Fullscreen:", this);
    commonLayout = new QGridLayout();
    commonLayout->addWidget(displayLabel, 0, 0);
    commonLayout->addWidget(display, 0, 1);
    commonLayout->addWidget(fullscreen, 1, 1, Qt::AlignTop);
    setLayout(commonLayout);
}

/* public slots */
QDomDocument Desktop_Graphics::getDevDocument() const
{
    return QDomDocument();
}
QString Desktop_Graphics::getDevDisplay() const
{
    return display->text();
}
QString Desktop_Graphics::getDevFullScreen() const
{
    return (fullscreen->isChecked())? "yes" : "no";
}
