#ifndef SPICE_VIEWER_H
#define SPICE_VIEWER_H

#include <QMainWindow>
#include <QDomDocument>
#include <QLabel>
#include <QTimerEvent>
#include "vm_viewer/vm_viewer.h"

class Spice_Viewer : public VM_Viewer
{
    Q_OBJECT
public:
    explicit Spice_Viewer(
            QWidget *parent = NULL,
            virConnect *conn = NULL,
            QString arg1 = QString(),
            QString arg2 = QString());
    ~Spice_Viewer();

signals:

private:
    QString      runXmlDesc;

public slots:

private slots:
    void         timerEvent(QTimerEvent*);
};

#endif // SPICE_VIEWER_H
