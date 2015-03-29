#ifndef CREATE_SNAPSHOT_FLAGS_H
#define CREATE_SNAPSHOT_FLAGS_H

#include <QMenuBar>
#include <QActionGroup>
#include <QDebug>

class CreateSnapshotFlags : public QMenu
{
    Q_OBJECT
public:
    explicit CreateSnapshotFlags(
            QWidget     *parent = NULL);

private:
    QActionGroup    *actGroup;

public slots:
    void             changeAvailableFlags(int);
    int              getCompositeFlag() const;

private slots:
    void             changeFlagCompatibility();
};

#endif // CREATE_SNAPSHOT_FLAGS_H
