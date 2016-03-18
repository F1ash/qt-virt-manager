#include "_changed.h"
#include <QTimer>

_Changed::_Changed(QWidget *parent) :
    QWidget(parent)
{
    setEnabled(false);
    QTimer::singleShot(333, this, SLOT(emitCompleteSignal()));
}
_Changed::~_Changed()
{

}

/* public slots */
void _Changed::stateChanged()
{
    emit dataChanged();
}
void _Changed::emitCompleteSignal()
{
    setEnabled(true);
    emit complete();
}
