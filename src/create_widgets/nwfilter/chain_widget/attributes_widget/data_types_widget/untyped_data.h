#ifndef UNTYPED_DATA_H
#define UNTYPED_DATA_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>

class UntypedData : public QWidget
{
    Q_OBJECT
public:
    explicit UntypedData(
            QWidget *parent = nullptr,
            QString  tag    = "");
    bool            isMatch() const;
    QString         getAttrName() const;
    virtual QString getAttrValue() const;
    void            addDataWidget(QWidget*);

signals:

private:
    QLabel          *name;
    QCheckBox       *match;
    QPushButton     *clear;
    QHBoxLayout     *commonLayout;

public slots:
    virtual void    clearData();
};

#endif // UNTYPED_DATA_H
