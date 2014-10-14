#ifndef INDEX_H
#define INDEX_H

#include <QObject>

class Index : public QObject
{
    Q_OBJECT
public:
    explicit Index(QObject *parent = NULL);

signals:

private:
    QString Name;
    QString State;
    QString Autostart;
    QString Persistent;

public slots:
    void setName(const QString&);
    void setState(const QString&);
    void setAutostart(const QString&);
    void setPersistent(const QString&);
    QString getName() const;
    QString getState() const;
    QString getAutostart() const;
    QString getPersistent() const;

};

#endif // INDEX_H
