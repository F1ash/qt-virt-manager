#ifndef GENERAL_H
#define GENERAL_H

#include "create_widgets/domain/_qwidget.h"
#include <QTextEdit>

class General : public _QWidget
{
    Q_OBJECT
public:
    explicit General(
            QWidget *parent = 0,
            QString  arg1 = "",
            QString  arg2 = "",
            QString  _xmlDesc = QString());

signals:

private:
    const QString    xmlDesc;
    QVBoxLayout     *commonLayout;
    QLabel          *typeLabel;
    QString          type;
    QLabel          *archLabel;
    QString          arch;
    QLabel          *nameLabel;
    QLineEdit       *name;
    QLabel          *uuidLabel;
    QLineEdit       *uuid;
    QLabel          *titleLabel;
    QLineEdit       *title;
    QLabel          *descLabel;
    QTextEdit       *description;

public slots:
    QDomDocument getDevDocument() const;

private slots:
    void readXMLDesciption();
};

#endif // GENERAL_H
