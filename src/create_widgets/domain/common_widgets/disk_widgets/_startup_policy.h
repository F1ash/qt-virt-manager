#ifndef _STARTUP_POLICY_H
#define _STARTUP_POLICY_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>

class _StartupPolicy : public QWidget
{
    Q_OBJECT
public:
    explicit _StartupPolicy(QWidget *parent = 0);

private:
    QCheckBox       *startupPolicyLabel;
    QComboBox       *startupPolicy;
    QHBoxLayout     *commonLayout;

public slots:
    bool    isUsed() const;
    QString getStartupPolicy() const;
};

#endif // _STARTUP_POLICY_H
