#ifndef _TAB_H
#define _TAB_H

#include <QMessageBox>
#include "create_widgets/common/_qwidget.h"
#include "create_widgets/domain/restore_panel.h"

class _Tab : public _QWidget
{
    Q_OBJECT
public:
    explicit _Tab(QWidget *parent = Q_NULLPTR);
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    RestorePanel    *restorePanel;
    QVBoxLayout     *commonLayout;

public slots:
    bool             closeDataEdit();
    void             stateChanged();
    virtual void     readXMLDesciption();
    virtual void     readXMLDesciption(const QString&);
    void             resetData();
    void             revertData();
    void             saveData();
};

#endif // _TAB_H
