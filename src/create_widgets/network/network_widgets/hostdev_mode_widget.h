#ifndef HOSTDEV_MODE_WIDGET_H
#define HOSTDEV_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class HOSTDEV_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit HOSTDEV_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // HOSTDEV_MODE_WIDGET_H
