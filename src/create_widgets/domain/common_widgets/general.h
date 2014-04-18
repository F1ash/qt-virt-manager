#ifndef GENERAL_H
#define GENERAL_H

#include "create_widgets/domain/_qwidget.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>

class General : public _QWidget
{
    Q_OBJECT
public:
    explicit General(QWidget *parent = 0, QString str = "");
    ~General();

signals:

private:
    QVBoxLayout     *commonLayout;
    QLabel          *typeLabel;
    QString          type;
    QLabel          *nameLabel;
    QLineEdit       *name;
    QLabel          *uuidLabel;
    QLabel          *uuid;
    QLabel          *titleLabel;
    QLineEdit       *title;
    QLabel          *descLabel;
    QTextEdit       *description;

public slots:
    QDomNodeList getNodeList() const;

};

#endif // GENERAL_H
