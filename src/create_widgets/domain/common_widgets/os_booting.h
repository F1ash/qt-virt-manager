#ifndef OS_BOOTING_H
#define OS_BOOTING_H

#include <QScrollArea>
#include <QMessageBox>
#include <QTextEdit>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"
#include "os_boot_widgets/_arch.h"

class OS_Booting : public _QWidget
{
    Q_OBJECT
public:
    explicit OS_Booting(
            QWidget *parent = NULL,
            QString  _caps  = "",
            QString  _xmlDesc = QString());
    _Arch           *architecture;

signals:
    void             archChanged(QString&);
    void             domainType(QString&);
    void             emulatorType(QString&);

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    QString          capabilities;
    QString          type;
    QString          arch;
    QString          os_type;
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
    void             readCapabilities();
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetSecData();
    void             revertSecData();
    void             saveSecData();
    void             changeOSType(QString&);
};

#endif // OS_BOOTING_H
