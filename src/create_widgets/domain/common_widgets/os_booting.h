#ifndef OS_BOOTING_H
#define OS_BOOTING_H

#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"

class OS_Booting : public _QWidget
{
    Q_OBJECT
public:
    explicit OS_Booting(
            QWidget *parent = NULL,
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    QTextEdit       *editor;
    RestorePanel    *restorePanel;

    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;
    QVBoxLayout     *commonLayout;

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

#endif // OS_BOOTING_H
