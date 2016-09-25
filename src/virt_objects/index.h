#ifndef INDEX_H
#define INDEX_H

#include <QObject>

class Index : public QObject
{
    Q_OBJECT
public:
    explicit Index(QObject *parent = nullptr);

signals:

private:
    QString Name;
    bool    State;
    QString State_EXT;
    bool    Autostart;
    bool    Persistent;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setState(const bool);
    void    setState_EXT(const QString&);
    void    setAutostart(const bool);
    void    setPersistent(const bool);
    void    setOnView(const bool);
    QString getName() const;
    bool    getState() const;
    QString getState_EXT() const;
    bool    getAutostart() const;
    bool    getPersistent() const;
    bool    getOnView() const;

};

#endif // INDEX_H
