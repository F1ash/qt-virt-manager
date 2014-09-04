#ifndef VIRTUAL_PORT_H
#define VIRTUAL_PORT_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include <QVBoxLayout>

typedef QMap<QString, QString> ParameterList;

class VirtualPort : public QWidget
{
    Q_OBJECT
public:
    explicit VirtualPort(QWidget *parent = 0);
    QLineEdit           *type;

private:
    QCheckBox           *useVirtPort;
    QLabel              *typeLabel, *managerIdLabel,
                        *typeIdLabel, *typeIdVerLabel,
                        *instanceIdLabel, *profileIdLabel;
    QLineEdit           *managerId, *typeId, *typeIdVer,
                        *instanceId, *profileId;
    QWidget             *parameters;
    QGridLayout         *parametersLayout;
    QVBoxLayout         *commonLayout;

public slots:
    ParameterList getParameterList() const;

private slots:
    void useingChanged(bool);
};

#endif // VIRTUAL_PORT_H
