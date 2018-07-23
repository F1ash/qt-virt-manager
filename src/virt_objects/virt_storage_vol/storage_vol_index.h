#ifndef STORAGE_VOL_INDEX_H
#define STORAGE_VOL_INDEX_H

#include <QObject>

class StorageVolIndex : public QObject
{
    Q_OBJECT
public:
    explicit StorageVolIndex(QObject *parent = Q_NULLPTR);

signals:

private:
    QString Name;
    QString Path;
    QString Type;
    QString CurrSize;
    QString LogicSize;

public slots:
    void setName(const QString&);
    void setPath(const QString&);
    void setType(const QString&);
    void setCurrSize(const QString&);
    void setLogicSize(const QString&);
    QString getName() const;
    QString getPath() const;
    QString getType() const;
    QString getCurrSize() const;
    QString getLogicSize() const;

};

#endif // STORAGE_VOL_INDEX_H
