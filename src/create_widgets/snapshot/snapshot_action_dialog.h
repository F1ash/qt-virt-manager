#ifndef SNAPSHOT_ACTION_DIALOG_H
#define SNAPSHOT_ACTION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTreeView>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "virt_objects/virt_entity_config.h"
#include "model/snapshot_tree_model.h"
#include <QDebug>

class SnapshotActionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SnapshotActionDialog(
            QWidget          *parent = NULL,
            virConnectPtr     currConnect = NULL,
            QString           _domName = QString());
    ~SnapshotActionDialog();

signals:

private:
    virConnectPtr       currJobConnect;
    virDomainPtr        domain = NULL;
    const QString       domName;
    QStringList         params;
    SnapshotTreeModel  *model;
    QAction            *revertAction;
    QAction            *deleteAction;
    QAction            *refreshAction;
    QTreeView          *snapshotTree;
    QToolBar           *toolBar;
    QHBoxLayout        *buttonsLayout;
    QVBoxLayout        *commonLayout;
    QLabel             *info;
    QPushButton        *ok;
    QPushButton        *cancel;
    QPushButton        *closeWdg;
    QWidget            *buttonsWdg;

public slots:
    QStringList         getParameters() const;

private slots:
    void                clearSnapshotTree();
    void                addSnapshotChild(int, const QModelIndex&, char*);
    void                setDomainSnapshots();
    void                accept();
    void                reject();
    void                cancelled();
    void                changeDialogState(bool);
    void                detectTriggeredAction(QAction*);
};

#endif // SNAPSHOT_ACTION_DIALOG_H
