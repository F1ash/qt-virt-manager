#ifndef ROUTE_MODE_WIDGET_H
#define ROUTE_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class ROUTE_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit ROUTE_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // ROUTE_MODE_WIDGET_H
