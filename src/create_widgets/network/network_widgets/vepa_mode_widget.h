#ifndef VEPA_MODE_WIDGET_H
#define VEPA_MODE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class VEPA_Mode_widget : public QWidget
{
    Q_OBJECT
public:
    explicit VEPA_Mode_widget(QWidget *parent = 0);

signals:

private:
    QVBoxLayout     *commonLayout;

public slots:

};

#endif // VEPA_MODE_WIDGET_H
