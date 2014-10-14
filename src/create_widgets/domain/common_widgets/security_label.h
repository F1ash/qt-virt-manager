#ifndef SECURITY_LABEL_H
#define SECURITY_LABEL_H

#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"

class SecurityLabel : public _QWidget
{
    Q_OBJECT
public:
    explicit SecurityLabel(
            QWidget *parent = NULL,
            QString  _xmlDesc = QString());

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;
    QCheckBox       *useSecLabel;
    QLabel          *typeLabel, *modelLabel,
                    *relabelLabel;
    QComboBox       *type, *model, *relabel,
                    *labelTypeLabel;
    QLineEdit       *label;
    QHBoxLayout     *typeLayout;
    QWidget         *typeWdg;
    QListWidget     *list;
    QPushButton     *add, *del;
    QGridLayout     *listLayout;
    QWidget         *listWdg;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    QDomDocument     getDataDocument() const;
    QString          closeDataEdit();

private slots:
    void             stateChanged();
    void             usedStateChanged(bool);
    void             securityTypeChanged(QString);
    void             modelTypeChanged(QString);
    QDomDocument     readData();
    void             addSecLabel();
    void             delSecLabel();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetSecData();
    void             revertSecData();
    void             saveSecData();
};

#endif // SECURITY_LABEL_H
