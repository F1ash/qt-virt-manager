#ifndef JOB_ITEM_INDEX_H
#define JOB_ITEM_INDEX_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>

typedef QMap<QString, QVariant> DATA;

class ConnItemIndex : public QObject
{
    Q_OBJECT
public:
    explicit ConnItemIndex(QObject *parent = 0);

signals:

private:
    QString  Name;
    QString  Host;
    QString  State;
    DATA     Data;

public slots:
    void setName(QString&);
    void setHost(QString&);
    void setState(QString&);
    void setData(DATA&);
    QString getName() const;
    QString getHost() const;
    QString getState() const;
    DATA    getData() const;

};

#endif // JOB_ITEM_INDEX_H
