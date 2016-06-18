#include "st_stacked_widget.h"
#include "dock_head_widgets/dock_widget.h"
#include <QMainWindow>
#include <QDebug>

ST_StackedWidget::ST_StackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    // hack: layout()->count() is crashed
    wdgCount = 0;
}

void ST_StackedWidget::addNewWidget(QWidget *w)
{
    int i = addWidget(w);
    wdgCount++;
    widget(i)->hide();
}
void ST_StackedWidget::removeAllWidgets()
{
    while ( wdgCount>0 ) {
        removeWidget(widget(0));
        --wdgCount;
    };
}

/* public slots */
void ST_StackedWidget::showDock(const QString &_dock)
{
    for ( uint i=0; i<wdgCount; i++ ) {
        QWidget *d =
                static_cast<QWidget*>(widget(i));
        if ( d!=nullptr ) {
            if ( _dock.startsWith(d->objectName()) ) {
                currentWidget()->hide();
                setCurrentIndex(i);
                d->show();
                break;
            };
        };
    };
    if ( !isVisible() ) show();
}
void ST_StackedWidget::showNextDock()
{
    int curr = currentIndex();
    ++curr;
    if ( curr>=wdgCount ) curr = 0;
    setCurrentIndex(curr);
}
void ST_StackedWidget::showPrevDock()
{
    int curr = currentIndex();
    --curr;
    if ( curr<0 ) curr = wdgCount-1;
    setCurrentIndex(curr);
}
