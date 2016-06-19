#include "dock_head_widget.h"

DockHeadWidget::DockHeadWidget(
        QWidget *parent, const QString _barName) :
    QWidget(parent), name(_barName)
{
    icon = new QLabel(this);
    nameWdg = new BarNameLabel(this);
    nameWdg->setText(name);
    floatIt = new QPushButton("", this);
    floatIt->setFlat(false);
    floatIt->setCheckable(true);
    commonLayout = new QVBoxLayout(this);
    commonLayout->setDirection(QBoxLayout::BottomToTop);
    commonLayout->addWidget(icon, 0, Qt::AlignBottom);
    commonLayout->addWidget(nameWdg);
    commonLayout->addStretch(-1);
    commonLayout->addWidget(floatIt, 0, Qt::AlignTop);
    connect(floatIt, SIGNAL(clicked(bool)),
            this, SIGNAL(floatChanged(bool)));
}

void DockHeadWidget::setFloatible(bool state)
{
    floatIt->setEnabled(state);
    QString t;
    if ( state ) {
        t.append("DoubleClick for floatible");
    } else {
        t.append("Wheel for change dock\nor\nuse Ctrl+Alt+Right/Left");
    };
    setToolTip(t);
}

/* public slots */
void DockHeadWidget::setTabBarName(const QString &_name)
{
    floatStateChanged(false);
    int _size = this->fontInfo().pixelSize();
    icon->setPixmap(
                QIcon::fromTheme(_name)
                .pixmap(_size));
    //floatIt->setMaximumSize(QSize(_size, _size));
    setLayout(commonLayout);
}
void DockHeadWidget::floatStateChanged(bool _floated)
{
    QString _icon, _toolTip;
    if (_floated) {
        _icon.append("dock_down");
        _toolTip.append("Push to dock");
    } else {
        _icon.append("dock_up");
        _toolTip.append("Undock");
    };
    floatIt->setIcon(QIcon::fromTheme(_icon));
    floatIt->setToolTip(_toolTip);
    floatIt->setChecked(_floated);
    nameWdg->setColor((_floated)? 0x008000:0x000080);
}

/* private slots */
void DockHeadWidget::wheelEvent(QWheelEvent *ev)
{
    ev->ignore();
    if ( floatIt->isEnabled() ) return;
    if ( ev->type()==QEvent::Wheel ) {
        if ( ev->delta()>0 ) {
            emit viewNextDock();
        } else if ( ev->delta()<0 ) {
            emit viewPrevDock();
        };
    };
}
