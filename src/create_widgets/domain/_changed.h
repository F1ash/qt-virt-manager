#ifndef _CHANGED_H
#define _CHANGED_H

#include <QWidget>
#include <QDebug>

class _Changed : public QWidget
{
    Q_OBJECT
public:
    explicit _Changed(QWidget *parent = NULL);

signals:
    void dataChanged();

private slots:
    void stateChanged();
};

#endif // _CHANGED_H
