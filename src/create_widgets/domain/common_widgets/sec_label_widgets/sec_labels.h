#ifndef SEC_LABELS_H
#define SEC_LABELS_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include "create_widgets/common/_qwidget.h"

class SecLabels : public _QWidget
{
    Q_OBJECT
public:
    explicit SecLabels(QWidget *parent = Q_NULLPTR);
    QComboBox       *type, *model, *relabel,
                    *labelTypeLabel;
    QLineEdit       *label;
    QListWidget     *list;

private:
    QCheckBox       *useSecLabel;
    QLabel          *typeLabel, *modelLabel,
                    *relabelLabel;
    QPushButton     *add, *del;
    QHBoxLayout     *typeLayout;
    QWidget         *typeWdg;
    QGridLayout     *listLayout;
    QWidget         *listWdg;
    QGridLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

public slots:
    bool             isUsed() const;
    void             setUsage(bool);
    QDomDocument     getDataDocument() const;
    void             readXMLDesciption(const QString&);

private slots:
    void             addSecLabel();
    void             delSecLabel();
    void             modelTypeChanged(QString);
    void             usedStateChanged(bool);
    void             securityTypeChanged(QString);
    QDomDocument     readData();
};

#endif // SEC_LABELS_H
