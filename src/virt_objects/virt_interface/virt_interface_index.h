#ifndef IFACE_INDEX_H
#define IFACE_INDEX_H

#include <QObject>

class Interface_Index : public QObject
{
    Q_OBJECT
public:
    explicit Interface_Index(QObject *parent = NULL);

signals:

private:
    QString Name;
    QString MAC;
    QString State;
    QString Changing;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setMAC(const QString&);
    void    setState(const QString&);
    void    setChanging(const QString&);
    void    setOnView(const bool);
    QString getName() const;
    QString getMAC() const;
    QString getState() const;
    QString getChanging() const;
    bool    getOnView() const;

};

#endif // IFACE_INDEX_H
