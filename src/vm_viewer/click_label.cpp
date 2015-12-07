#include "click_label.h"

Click_Label::Click_Label(QWidget *parent)
{

}

void Click_Label::mouseReleaseEvent(QMouseEvent *ev)
{
    if ( ev->type()==QEvent::MouseButtonRelease ) {
        emit released();
    };
}
