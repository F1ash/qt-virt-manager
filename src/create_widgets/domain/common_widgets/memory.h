#ifndef MEMORY_H
#define MEMORY_H

#include <QScrollArea>
#include <QMessageBox>
#include "create_widgets/domain/_qwidget.h"
#include "memory_widgets/hugepages.h"
#include "create_widgets/domain/restore_panel.h"

class Memory : public _QWidget
{
    Q_OBJECT
public:
    explicit Memory(
            QWidget *parent = NULL,
            QString  _caps  = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    bool             currentStateSaved = true;
    QString          currentDeviceXMLDesc;
    QString          capabilities;
    QString          memUnit;
    QString          memValue;
    RestorePanel    *restorePanel;
    QLabel          *hostMemory;
    QLabel          *maxMemLabel;
    QLabel          *currMemLabel;
    QSpinBox        *maxMemValue;
    QSpinBox        *currMemValue;
    QGridLayout     *memLayout;
    QWidget         *memWdg;

    QCheckBox       *enableMemBacking;
    QCheckBox       *hugepages;
    QCheckBox       *nosharepages;
    HugePages       *hugePagesList;
    QCheckBox       *locked;
    QGridLayout     *memBackingLayout;
    QWidget         *memBackingWdg;

    QCheckBox       *enableMemTune;
    QLabel          *hardLabel;
    QSpinBox        *hard_limit;
    QLabel          *softLabel;
    QSpinBox        *soft_limit;
    QLabel          *swapLabel;
    QSpinBox        *swap_hard_limit;
    QLabel          *guaranteeLabel;
    QSpinBox        *min_guarantee;
    QGridLayout     *memTuneLayout;
    QWidget         *memTuneWdg;

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
    void             changeCurrentMemValue(int);
    void             changeMaximumMemValue(int);
    void             readXMLDesciption();
    void             readXMLDesciption(QString&);
    quint64          convertNiBtoKiB(quint64, QString&);
    void             resetMemData();
    void             revertMemData();
    void             saveMemData();
};

#endif // MEMORY_H
