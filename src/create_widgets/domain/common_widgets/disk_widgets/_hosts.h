#ifndef _HOSTS_H
#define _HOSTS_H

#include <QWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

class _Hosts : public QWidget
{
    Q_OBJECT
public:
    explicit _Hosts(QWidget *parent = 0);

private:
    QCheckBox       *useHosts;
    QListWidget     *hosts;
    QPushButton     *add;
    QPushButton     *del;
    QLineEdit       *name, *port;
    QLabel          *colon;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;
    QVBoxLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    bool        isUsed() const;
    QStringList getHostsList() const;

private slots:
    void addHost();
    void delHost();
};

#endif // _HOSTS_H
