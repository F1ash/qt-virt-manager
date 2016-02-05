#ifndef FIND_SECRET_DIALOG_H
#define FIND_SECRET_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include "virt_objects/virt_secret/secret_control.h"
#include "virt_objects/virt_secret/secret_control_thread.h"

struct FSD_Result {
    QString uuid    = QString();
    QString usage   = QString();
    QString type    = QString();
};

class FindSecretDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FindSecretDialog(
            QWidget        *parent  = NULL,
            virConnectPtr*  connPtrPtr = NULL);

private:
    VirtSecretControl   *secrets;
    QHBoxLayout         *listLayout;
    QWidget             *listWidget;
    QPushButton         *chooseSecret;
    QPushButton         *cancel;
    QHBoxLayout         *buttonLayout;
    QWidget             *buttons;
    QVBoxLayout         *commonLayout;
    SecretControlThread *thread;

    virConnectPtr*       ptr_ConnPtr;

public slots:
    FSD_Result           getResult() const;
    void                 showMsg(QString&);

private slots:
    void                 setSecretList();
    void                 set_Result();
    void                 execAction(TASK);
};

#endif // FIND_SECRET_DIALOG_H
