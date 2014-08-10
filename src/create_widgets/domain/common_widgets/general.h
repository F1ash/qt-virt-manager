#ifndef GENERAL_H
#define GENERAL_H

#include "create_widgets/domain/_qwidget.h"
#include <QTextEdit>

class General : public _QWidget
{
    Q_OBJECT
public:
    explicit General(QWidget *parent = 0, QString arg1 = "", QString arg2 = "", QString arg3 = "");
    ~General();

signals:

private:
    QVBoxLayout     *commonLayout;
    QLabel          *typeLabel;
    QString          type;
    QLabel          *archLabel;
    QString          arch;
    QLabel          *emulLabel;
    QString          emulator;
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
