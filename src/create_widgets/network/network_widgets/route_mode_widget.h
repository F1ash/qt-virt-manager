#ifndef ROUTE_MODE_WIDGET_H
#define ROUTE_MODE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

class ROUTE_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit ROUTE_Mode_widget(QWidget *parent = 0);
    QLineEdit       *dev;

private:
    QLabel          *devLabel;
    QHBoxLayout     *devLayout;
    QWidget         *devWdg;
    QVBoxLayout     *commonLayout;
};

#endif // ROUTE_MODE_WIDGET_H
