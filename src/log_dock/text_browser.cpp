#include "text_browser.h"

#define  HNAVY_SIZE 64

TextBrowser::TextBrowser(QWidget *parent) :
    QTextBrowser(parent)
{
    prevL = new HNavigationLabel(this);
    prevL->setToolTip(tr("Previous dock"));
    nextL = new HNavigationLabel(this);
    nextL->setToolTip(tr("Next dock"));
    next = QIcon::fromTheme("next")
            .pixmap(HNAVY_SIZE);
    prev = QIcon::fromTheme("previous")
            .pixmap(HNAVY_SIZE);
    prevL->setPixmap(prev);
    nextL->setPixmap(next);
    prevL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);
    nextL->setBaseSize(HNAVY_SIZE, HNAVY_SIZE);

    setReadOnly(true);
    setOpenLinks(false);
    setOpenExternalLinks(true);
    setContextMenuPolicy(Qt::DefaultContextMenu);
}

void TextBrowser::resizeEvent(QResizeEvent *ev)
{
    ev->accept();
    prevL->move(
                0,
                ev->size().height()-HNAVY_SIZE);
    nextL->move(
                ev->size().width()-HNAVY_SIZE,
                ev->size().height()-HNAVY_SIZE);
}
