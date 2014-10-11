#ifndef HUGEPAGES_H
#define HUGEPAGES_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class HugePages : public QWidget
{
    Q_OBJECT
public:
    explicit HugePages(QWidget *parent = 0);
    QListWidget     *pages;
    QSpinBox        *size;
    QComboBox       *units;
    QLineEdit       *nodeset;
    QPushButton     *add;
    QPushButton     *del;
    QHBoxLayout     *panelLayout;
    QWidget         *panel;
    QVBoxLayout     *baseLayout;
    QWidget         *baseWdg;
    QVBoxLayout     *commonLayout;

signals:
    void             dataChanged();

public slots:
    QStringList      getPagesList() const;
    void             setPageList(QString&);

private slots:
    void             addPage();
    void             delPage();
};

#endif // HUGEPAGES_H
