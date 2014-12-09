#ifndef PRIVATE_MODE_WIDGET_H
#define PRIVATE_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class PRIVATE_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit PRIVATE_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // PRIVATE_MODE_WIDGET_H
