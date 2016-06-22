#ifndef TEXT_BROWSER_H
#define TEXT_BROWSER_H

#include <QTextBrowser>
#include <QPixmap>
#include <QIcon>
#include <QResizeEvent>
#include <QPaintEvent>
#include "virt_objects/h_navigation_label.h"

class TextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit TextBrowser(QWidget *parent = nullptr);
    HNavigationLabel   *prevL, *nextL, *homeL;

private:
    QPixmap             next, prev, home;

private slots:
    void                resizeEvent(QResizeEvent*);
    //void                paintEvent(QPaintEvent*);
};

#endif // TEXT_BROWSER_H
