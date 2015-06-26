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
    QString Name;
    QString State;
    bool    onView;

public slots:
    void    setName(const QString&);
    void    setState(const QString&);
    void    setOnView(const bool);
    QString getName() const;
    QString getState() const;
    bool    getOnView() const;

};

#endif // SECRET_INDEX_H
