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

    bool             hostMode = false;
    bool             oneHostMode = false;

public slots:
    bool        isUsed() const;
    void        checkHosts(bool);
    void        setFullHostMode(bool);
    QStringList getHostsList() const;
    void        setOneHostMode(bool);

private slots:
    void addHost();
    void delHost();
};

#endif // _HOSTS_H
