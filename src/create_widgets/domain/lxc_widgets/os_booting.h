#ifndef LXC_OS_BOOTING_H
#define LXC_OS_BOOTING_H

#include "create_widgets/domain/_qwidget.h"
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>

class LXC_OSBooting : public _QWidget
{
    Q_OBJECT
public:
    explicit LXC_OSBooting(QWidget *parent = 0);
    ~LXC_OSBooting();

signals:

private:
    QLabel          *arch;
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
