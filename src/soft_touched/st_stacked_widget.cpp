#include "st_stacked_widget.h"
#include "dock_head_widgets/dock_widget.h"
#include <QMainWindow>
#include <QDebug>

ST_StackedWidget::ST_StackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    // hack: layout()->count() is crashed
    wdgCount = 0;
    showAnimation = nullptr;
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
    for ( int i=0; i<wdgCount; i++ ) {
        QWidget *d =
                static_cast<QWidget*>(widget(i));
        if ( d!=nullptr ) {
            if ( _dock.startsWith(d->objectName()) ) {
                currentWidget()->hide();
                d->show();
                animatedDockShow(i);
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
    animatedDockShow(curr);
}
void ST_StackedWidget::showPrevDock()
{
    int curr = currentIndex();
    --curr;
    if ( curr<0 ) curr = wdgCount-1;
    animatedDockShow(curr);
}

/* private slots */
void ST_StackedWidget::animatedDockShow(int i)
{
    setCurrentIndex(i);
    QWidget *d =
            static_cast<QWidget*>(widget(i));
    if ( d!=nullptr ) {
        if ( showAnimation!=nullptr ) {
            delete showAnimation;
            showAnimation = nullptr;
        };
        d->setWindowFlags(
                    Qt::Popup | Qt::FramelessWindowHint);
        //d->setWindowOpacity(0.667);
        showAnimation = new QPropertyAnimation(
                    d, "pos");
        showAnimation->setDuration(333);
        showAnimation->setStartValue(
                    mapToGlobal(QPoint(10, 10)));
        showAnimation->setEndValue(
                    mapToGlobal(QPoint(0, 0)));
        connect(showAnimation, SIGNAL(finished()),
                this, SLOT(animatedDockShowComlete()));
        showAnimation->start();
        d->show();
    };
}
void ST_StackedWidget::animatedDockShowComlete()
{
    QPropertyAnimation *a =
            static_cast<QPropertyAnimation*>(sender());
    if ( a!=nullptr ) {
        QWidget *d =
                static_cast<QWidget*>(a->targetObject());
        if ( d!=nullptr ) {
            d->setWindowFlags(Qt::Widget);
            disconnect(showAnimation, SIGNAL(finished()),
                       this, SLOT(animatedDockShowComlete()));
            d->show();
            //d->setWindowOpacity(1.0);
        };
    };
}
