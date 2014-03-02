#ifndef DOMAIN_CONTROL_H
#define DOMAIN_CONTROL_H

#include <QMainWindow>

class VirtDomainControl : public QMainWindow
{
    Q_OBJECT
public:
    explicit VirtDomainControl(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // DOMAIN_CONTROL_H
