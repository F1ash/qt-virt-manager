#ifndef PASSTHROUGH_MODE_WIDGET_H
#define PASSTHROUGH_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class PASSTHROUGH_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit PASSTHROUGH_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // PASSTHROUGH_MODE_WIDGET_H
