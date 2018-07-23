#ifndef _STARTUP_POLICY_H
#define _STARTUP_POLICY_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include "create_widgets/common/_changed.h"

class _StartupPolicy : public _Changed
{
    Q_OBJECT
public:
    explicit _StartupPolicy(QWidget *parent = Q_NULLPTR);

signals:
    //void             dataChanged();

private:
    QCheckBox       *startupPolicyLabel;
    QComboBox       *startupPolicy;
    QHBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    QString          getStartupPolicy() const;
    int              findPolicyIndex(QString&);
    void             setPolicyIndex(int);
};

#endif // _STARTUP_POLICY_H
