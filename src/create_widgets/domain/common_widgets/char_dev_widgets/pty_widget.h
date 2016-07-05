#ifndef PTY_WIDGET_H
#define PTY_WIDGET_H

#include "create_widgets/domain/_qwidget.h"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>

class PtyWidget : public _QWidget
{
    Q_OBJECT
public:
    explicit PtyWidget(QWidget *parent=0, QString _tag = "");

private:
    QGridLayout     *ptyLayout;
    QString          tag;

public slots:
    QDomDocument     getDataDocument() const;
    void             setDataDescription(const QString&);
};

#endif // PTY_WIDGET_H
