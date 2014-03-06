#ifndef STORAGE_VOL_INDEX_H
#define STORAGE_VOL_INDEX_H

#include <QObject>

class StorageVolIndex : public QObject
{
    Q_OBJECT
public:
    explicit StorageVolIndex(QObject *parent = 0);

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

#endif // STORAGE_VOL_INDEX_H
