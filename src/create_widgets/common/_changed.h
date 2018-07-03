#ifndef _CHANGED_H
#define _CHANGED_H

#include <QWidget>
//#include <QDebug>

class _Changed : public QWidget
{
    Q_OBJECT
public:
    explicit _Changed(QWidget *parent = nullptr);
    virtual ~_Changed()               = 0;

signals:
    void dataChanged();
    void complete();

public slots:
    virtual void stateChanged();
    virtual void emitCompleteSignal();
};

#endif // _CHANGED_H
