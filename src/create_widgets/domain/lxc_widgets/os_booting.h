#ifndef LXC_OS_BOOTING_H
#define LXC_OS_BOOTING_H

#include "create_widgets/domain/_qwidget.h"

class LXC_OSBooting : public _QWidget
{
    Q_OBJECT
public:
    explicit LXC_OSBooting(QWidget *parent = 0, QString arg1 = "???", QString arg2 = "???");
    ~LXC_OSBooting();

signals:

private:
    QString          arch;
    QString          os_type;
    QLabel          *initPathLabel;
    QLineEdit       *initPath;

    QLabel          *startLabel;
    QLabel          *targetLabel;
    QLabel          *countLabel;
    QLabel          *uidStart;
    QLabel          *gidStart;
    QLabel          *uidLabel;
    QLabel          *gidLabel;
    QSpinBox        *uidTarget;
    QSpinBox        *gidTarget;
    QSpinBox        *uidCount;
    QSpinBox        *gidCount;
    QCheckBox       *nameSpaceEnable;
    QGridLayout     *nameSpaceLayout;
    QWidget         *nameSpaceWidget;

    QVBoxLayout     *commonLayout;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // LXC_OS_BOOTING_H
