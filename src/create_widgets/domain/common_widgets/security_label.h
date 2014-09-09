#ifndef SECURITY_LABEL_H
#define SECURITY_LABEL_H

#include <QListWidget>
#include <QPushButton>
#include "create_widgets/domain/_qwidget.h"

class SecurityLabel : public _QWidget
{
    Q_OBJECT
public:
    explicit SecurityLabel(QWidget *parent = 0);

private:
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
    QDomDocument    getDevDocument() const;

private slots:
    void usedStateChanged(bool);
    void securityTypeChanged(QString);
    void modelTypeChanged(QString);
    QDomDocument readData();
    void addSecLabel();
    void delSecLabel();
};

#endif // SECURITY_LABEL_H
