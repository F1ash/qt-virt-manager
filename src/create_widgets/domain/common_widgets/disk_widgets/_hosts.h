#ifndef _HOSTS_H
#define _HOSTS_H

#include "create_widgets/common/_list_widget.h"

class _Hosts : public _List_Widget
{
    Q_OBJECT
public:
    explicit _Hosts(
            QWidget *parent = nullptr,
            QString  tag    = "");

private:
    QLineEdit       *name, *port;

    bool             hostMode = false;
    bool             oneHostMode = false;

public slots:
    void             setFullHostMode(bool);
    QStringList      getHostsList() const;
    void             setOneHostMode(bool);
    void             setHostItem(QString&);
    void             clearHostList();
    void             setHostPlaceholderText(const QString&);
    void             setPortPlaceholderText(const QString&);

private slots:
    void             addItem();
    void             delItem();
};

#endif // _HOSTS_H
