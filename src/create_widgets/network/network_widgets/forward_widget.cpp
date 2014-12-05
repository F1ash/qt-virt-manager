#include "forward_widget.h"

#define FORWARD_MODE QStringList()\
    <<"nat"\
    <<"route"\
    <<"bridge"\
    <<"private"\
    <<"vepa"\
    <<"passthrough"\
    <<"hostdev"

Forward_Widget::Forward_Widget(QWidget *parent) :
    _QWidget(parent)
{
    title = new QCheckBox("Forwards", this);
    modeLabel = new QLabel("Mode:", this);
    mode = new QComboBox(this);
    mode->addItems(FORWARD_MODE);
    frwdLayout = new QGridLayout();
    frwdLayout->addWidget(modeLabel, 0, 0);
    frwdLayout->addWidget(mode, 0, 1);
    forwards = new QWidget(this);
    forwards->setLayout(frwdLayout);
    forwards->setEnabled(false);
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(title);
    commonLayout->addWidget(forwards);
    commonLayout->addStretch(-1);
    setLayout(commonLayout);
    connect(title, SIGNAL(toggled(bool)),
            forwards, SLOT(setEnabled(bool)));
}
