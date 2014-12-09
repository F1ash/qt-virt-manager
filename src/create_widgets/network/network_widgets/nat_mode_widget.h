#ifndef NAT_MODE_WIDGET_H
#define NAT_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class NAT_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit NAT_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // NAT_MODE_WIDGET_H
