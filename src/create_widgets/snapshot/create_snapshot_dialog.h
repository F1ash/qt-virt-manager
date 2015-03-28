#ifndef CREATE_SNAPSHOT_DIALOG_H
#define CREATE_SNAPSHOT_DIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QTimerEvent>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "types/mem_state_snapshot.h"
#include "types/disk_snapshot.h"
#include "types/system_checkpoint.h"
#include <QDebug>

class CreateSnapshotDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateSnapshotDialog(
            QWidget *parent = NULL,
            QString  domainName = QString(),
            bool     _state = false);

private:
    QHBoxLayout     *titleLayout;
    QVBoxLayout     *baseLayout;
    QHBoxLayout     *buttonsLayout;
    QVBoxLayout     *commonLayout;
    QComboBox       *snapshotType;
    QLabel          *nameLabel, *timeLabel, *info;
    QLineEdit       *name;
    QCheckBox       *addTimeSuff;
    QLineEdit       *description;
    QWidget         *titleWdg;
    QStackedWidget  *baseWdg;
    QPushButton     *ok;
    QPushButton     *cancel;
    QWidget         *buttonsWdg;
    int              timerID;

public slots:
    char*            getSnapshotXMLDesc() const;
    QString          getSnapshotType() const;

private slots:
    void             accept();
    void             reject();
    void             snapshotTypeChange(int);
    void             timerEvent(QTimerEvent*);
};

#endif // CREATE_SNAPSHOT_DIALOG_H
