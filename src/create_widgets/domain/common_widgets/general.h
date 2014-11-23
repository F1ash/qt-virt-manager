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
            QString  _caps  = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;
    QVBoxLayout     *commonLayout;
    QString          capabilities;
    QLabel          *typeLabel;
    QLabel          *archLabel;
    QString          type;
    QString          arch;
    QString          os_type;
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
    void             changeArch(QString&);

private slots:
    void             readCapabilities();
    void             stateChanged();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    void             resetGenData();
    void             revertGenData();
    void             saveGenData();
};

#endif // GENERAL_H
