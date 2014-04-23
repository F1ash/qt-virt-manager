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
    QString Path;
    QString Type;
    QString Size;

public slots:
    void setName(const QString&);
    void setPath(const QString&);
    void setType(const QString&);
    void setSize(const QString&);
    QString getName() const;
    QString getPath() const;
    QString getType() const;
    QString getSize() const;

};

#endif // STORAGE_VOL_INDEX_H
