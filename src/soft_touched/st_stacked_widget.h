#ifndef ST_STACKED_WIDGET_H
#define ST_STACKED_WIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>

class ST_StackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ST_StackedWidget(QWidget *parent = Q_NULLPTR);
    void        addNewWidget(QWidget *);
    void        removeAllWidgets();

private:
    int         wdgCount;
    QPropertyAnimation
               *showAnimation;

public slots:
    void        showDock(const QString&);
    void        showNextDock();
    void        showPrevDock();

private slots:
    void        animatedDockShow(int);
    void        animatedDockShowComlete();
};

#endif // ST_STACKED_WIDGET_H
