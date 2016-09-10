#include "_attributes.h"

_Attributes::_Attributes(QWidget *parent, QString tag) :
    _QWidget(parent)
{
    attrLabel = new QLabel(tag, this);
    attrName = new QComboBox(this);
    attrLayout = new QHBoxLayout(this);
    attrLayout->addWidget(attrLabel);
    attrLayout->addWidget(attrName);
    attrWdg = new QWidget(this);
    attrWdg->setLayout(attrLayout);
    attrEditor = new QStackedWidget(this);

    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(attrWdg);
    commonLayout->addWidget(attrEditor);
    setLayout(commonLayout);

    connect(attrName, SIGNAL(currentIndexChanged(int)),
            attrEditor, SLOT(setCurrentIndex(int)));
}
void _Attributes::clearAllAttributeData()
{
    for (uint i=0; i<attrEditor->count(); i++) {
        UntypedData *d = static_cast<UntypedData*>(
                    attrEditor->widget(i));
        if ( d==nullptr ) continue;
        d->clearData();
    };
}
