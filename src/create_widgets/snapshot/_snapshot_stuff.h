#ifndef _SNAPSHOT_STUFF_H
#define _SNAPSHOT_STUFF_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QDomDocument>
#include <QLineEdit>
#include <QDebug>

class _SnapshotStuff : public QWidget
{
    Q_OBJECT
public:
    explicit _SnapshotStuff(
            QWidget *parent = NULL,
            bool     _state = false);
    int                      type;
    const bool               state;
    QLabel                  *memLabel, *diskWarn,
                            *filePathLabel;
    QLineEdit               *filePath;
    QVBoxLayout             *commonLayout;
    virtual QDomDocument     getElements() const;

private:

public slots:
};

#endif // _SNAPSHOT_STUFF_H
