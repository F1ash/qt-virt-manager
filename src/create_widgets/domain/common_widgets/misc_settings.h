#ifndef MISC_SETTINGS_H
#define MISC_SETTINGS_H

#include <QScrollArea>
#include <QMessageBox>
#include "create_widgets/domain/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"
#include "misc_settings_widgets/events.h"
#include "misc_settings_widgets/power.h"
#include "misc_settings_widgets/hv_features.h"

class Misc_Settings : public _QWidget
{
    Q_OBJECT
public:
    explicit Misc_Settings(
            QWidget *parent = NULL,
            QString  _caps  = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    QString          capabilities;
    QString          type;
    QLabel          *eventsLabel;
    QCheckBox       *powerLabel, *featuresLabel;
    Events          *eventsWdg;
    Power           *powerWdg;
    HV_Features     *featuresWdg;
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
    void             resetMiscData();
    void             revertMiscData();
    void             saveMiscData();
};

#endif // MISC_SETTINGS_H
