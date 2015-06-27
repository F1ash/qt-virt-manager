#ifndef SECRET_INDEX_H
#define SECRET_INDEX_H

#include <QObject>

class Secret_Index : public QObject
{
    Q_OBJECT
public:
    explicit Secret_Index(QObject *parent = NULL);

signals:

private:
    QString UsageID;
    QString UUID;
    QString Type;
    bool    onView;

public slots:
    void    setUsageID(const QString&);
    void    setUUID(const QString&);
    void    setType(const QString&);
    void    setOnView(const bool);
    QString getUsageID() const;
    QString getUUID() const;
    QString getType() const;
    bool    getOnView() const;

};

#endif // SECRET_INDEX_H
