#ifndef RANDOM_EDIT_H
#define RANDOM_EDIT_H

#include "create_widgets/domain/common_widgets/random.h"

class Random_Edit : public Random
{
    Q_OBJECT
public:
    explicit Random_Edit(QWidget *parent = 0);

signals:

public slots:
    void setDataDescription(QString&);
};

#endif // RANDOM_EDIT_H
