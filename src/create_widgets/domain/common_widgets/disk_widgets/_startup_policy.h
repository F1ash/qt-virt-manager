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
    explicit _StartupPolicy(QWidget *parent = NULL);

signals:
    void             dataChanged();

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
