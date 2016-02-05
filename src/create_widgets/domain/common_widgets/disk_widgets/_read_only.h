#ifndef _READ_ONLY_H
#define _READ_ONLY_H

#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include "create_widgets/domain/_changed.h"

class _ReadOnly : public _Changed
{
    Q_OBJECT
public:
    explicit _ReadOnly(QWidget *parent = NULL);
    QCheckBox       *readOnly;

signals:
    //void             dataChanged();

private:
    QHBoxLayout     *commonLayout;

public slots:
    bool             state() const;
};

#endif // _READ_ONLY_H
