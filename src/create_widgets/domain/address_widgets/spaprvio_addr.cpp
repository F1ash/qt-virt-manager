#include "spaprvio_addr.h"

SpaprVioAddr::SpaprVioAddr(QWidget *parent) :
    _Addr(parent)
{
    regLabel = new QLabel("Reg:", this);
    reg = new QLineEdit(this);
    reg->setPlaceholderText("the hex value address of the starting register");
    commonlayout = new QGridLayout();
    commonlayout->addWidget(regLabel, 0, 0);
    commonlayout->addWidget(reg, 0, 1);
    setLayout(commonlayout);
    connect(reg, SIGNAL(textEdited(QString)),
            this, SLOT(stateChanged()));
}
AttrList SpaprVioAddr::getAttrList() const
{
    AttrList attrs;
    if ( !reg->text().isEmpty() )
        attrs.insert("reg", reg->text());
    return attrs;
}
