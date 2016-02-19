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
    QString State;
    QString Autostart;
    QString Persistent;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setState(const QString&);
    void    setAutostart(const QString&);
    void    setPersistent(const QString&);
    void    setOnView(const bool);
    QString getName() const;
    QString getState() const;
    QString getAutostart() const;
    QString getPersistent() const;
    bool    getOnView() const;

};

#endif // INDEX_H
