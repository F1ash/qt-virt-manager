#ifndef NWFILTER_INDEX_H
#define NWFILTER_INDEX_H

#include <QObject>

class NWFilter_Index : public QObject
{
    Q_OBJECT
public:
    explicit NWFilter_Index(QObject *parent = Q_NULLPTR);

signals:

private:
    QString Name;
    QString UUID;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setUUID(const QString&);
    void    setOnView(const bool);
    QString getName() const;
    QString getUUID() const;
    bool    getOnView() const;

};

#endif // NWFILTER_INDEX_H
