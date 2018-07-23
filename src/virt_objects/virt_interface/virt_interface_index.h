#ifndef IFACE_INDEX_H
#define IFACE_INDEX_H

#include <QObject>

class Interface_Index : public QObject
{
    Q_OBJECT
public:
    explicit Interface_Index(QObject *parent = Q_NULLPTR);

signals:

private:
    QString Name;
    QString MAC;
    bool    State;
    bool    Changing;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setMAC(const QString&);
    void    setState(const bool);
    void    setChanging(const bool);
    void    setOnView(const bool);
    QString getName() const;
    QString getMAC() const;
    bool    getState() const;
    bool    getChanging() const;
    bool    getOnView() const;

};

#endif // IFACE_INDEX_H
