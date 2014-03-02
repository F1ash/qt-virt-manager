#ifndef VIRTNET_INDEX_H
#define VIRTNET_INDEX_H

#include <QObject>

class VirtNetIndex : public QObject
{
    Q_OBJECT
public:
    explicit VirtNetIndex(QObject *parent = 0);

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

#endif // VIRTNET_INDEX_H
