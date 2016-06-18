#ifndef ST_STACKED_WIDGET_H
#define ST_STACKED_WIDGET_H

#include <QStackedWidget>

class ST_StackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit ST_StackedWidget(QWidget *parent = nullptr);
    void        addNewWidget(QWidget *);
    void        removeAllWidgets();

private:
    int         wdgCount;

public slots:
    void        showDock(const QString&);
    void        showNextDock();
    void        showPrevDock();
};

#endif // ST_STACKED_WIDGET_H
