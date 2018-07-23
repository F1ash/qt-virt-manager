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
            QWidget *parent = Q_NULLPTR,
            QString  tag    = "");
    void             setMatchUnusable();
    bool             isMatchUnusable() const;
    bool             isMatch() const;
    void             matchToDefault();
    void             setMatchState(bool);
    QString          getAttrName() const;
    virtual QString  getAttrValue() const;
    virtual void     setAttrValue(const QString&);
    void             addDataWidget(QWidget*);

signals:
    void             dataChanged();

private:
    QLabel          *name;
    QCheckBox       *match;
    QPushButton     *clear;
    QHBoxLayout     *commonLayout;

public slots:
    virtual void     clearData();
};

#endif // UNTYPED_DATA_H
