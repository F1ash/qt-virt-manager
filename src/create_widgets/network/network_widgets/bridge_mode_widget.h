#ifndef BRIDGE_MODE_WIDGET_H
#define BRIDGE_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class BRIDGE_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit BRIDGE_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // BRIDGE_MODE_WIDGET_H
