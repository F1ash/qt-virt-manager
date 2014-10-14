#ifndef GENERAL_H
#define GENERAL_H

#include "create_widgets/domain/_qwidget.h"
#include <QTextEdit>
#include <QMessageBox>
#include "create_widgets/domain/restore_panel.h"

class General : public _QWidget
{
    Q_OBJECT
public:
    explicit General(
            QWidget *parent = NULL,
            QString  arg1 = "",
            QString  arg2 = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;
    QVBoxLayout     *commonLayout;
    QLabel          *typeLabel;
    QString          type;
    QLabel          *archLabel;
    QString          arch;
    QLabel          *nameLabel;
    QLineEdit       *name;
    QLabel          *uuidLabel;
    QLineEdit       *uuid;
    QLabel          *titleLabel;
    QLineEdit       *title;
    QLabel          *descLabel;
    QTextEdit       *description;

public slots:
    QDomDocument     getDataDocument() const;
    QString          closeDataEdit();

private slots:
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetSecData();
    void             revertSecData();
    void             saveSecData();
};

#endif // GENERAL_H
