#ifndef DOMAIN_DOC_H
#define DOMAIN_DOC_H

#include <QMainWindow>

class DomainDocContent : public QMainWindow
{
    Q_OBJECT
public:
    explicit DomainDocContent(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // DOMAIN_DOC_H
