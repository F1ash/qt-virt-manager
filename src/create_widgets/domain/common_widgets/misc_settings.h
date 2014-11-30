#ifndef MISC_SETTINGS_H
#define MISC_SETTINGS_H

#include <QScrollArea>
#include "create_widgets/domain/_tab.h"
#include "misc_settings_widgets/events.h"
#include "misc_settings_widgets/power.h"
#include "misc_settings_widgets/hv_features.h"

class Misc_Settings : public _Tab
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
    QString          capabilities;
    QString          type;
    QLabel          *eventsLabel;
    QCheckBox       *powerLabel, *featuresLabel;
    Events          *eventsWdg;
    Power           *powerWdg;
    HV_Features     *featuresWdg;

    QScrollArea     *commonWdg;
    QVBoxLayout     *scrolledLayout;
    QWidget         *scrolled;

public slots:
    QDomDocument     getDataDocument() const;

private slots:
    void             readCapabilities();
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
};

#endif // MISC_SETTINGS_H
