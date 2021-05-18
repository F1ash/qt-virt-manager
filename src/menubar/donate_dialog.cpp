#include "donate_dialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QClipboard>

Donate_Dialog::Donate_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Donate"));
    //donate1 = new Click_Label(this);
    //donate1->setToolTip(tr("Donate on Yandex.Money"));
    //donate1->setOpenExternalLinks(true);
    //donate1->setPixmap(QIcon::fromTheme("yandex_money").pixmap(128));
    donate2 = new Click_Label(this);
    donate2->setToolTip(tr("Donate on MasterCard\nCopy card number to Clipboard"));
    donate2->setOpenExternalLinks(true);
    donate2->setPixmap(QIcon::fromTheme("mastercard").pixmap(128));
    //donate3 = new Click_Label(this);
    //donate3->setToolTip(tr("Donate Bitcoin\nCopy to Clipboard"));
    //donate3->setOpenExternalLinks(true);
    //donate3->setPixmap(QIcon::fromTheme("electrum").pixmap(128));
    //donate4 = new Click_Label(this);
    //donate4->setToolTip(tr("Donate BitcoinCash\nCopy to Clipboard"));
    //donate4->setOpenExternalLinks(true);
    //donate4->setPixmap(QIcon::fromTheme("electron-cash").pixmap(128));

    donateLayout = new QGridLayout(this);
    //donateLayout->addWidget(donate1, 0, 0, Qt::AlignCenter);
    donateLayout->addWidget(donate2, 0, 1, Qt::AlignCenter);
    //donateLayout->addWidget(donate3, 1, 0, Qt::AlignCenter);
    //donateLayout->addWidget(donate4, 1, 1, Qt::AlignCenter);
    donateWdg = new QWidget(this);
    donateWdg->setLayout(donateLayout);
    //again = new QCheckBox(tr("don't show again"), this);
    //again->hide();
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(donateWdg);
    //commonLayout->addWidget(again);
    setLayout(commonLayout);

    //connect(donate1, SIGNAL(released()),
    //        this, SLOT(donationClicked()));
    connect(donate2, SIGNAL(released()),
            this, SLOT(donationClicked()));
    //connect(donate3, SIGNAL(released()),
    //        this, SLOT(donationClicked()));
    //connect(donate4, SIGNAL(released()),
    //        this, SLOT(donationClicked()));
}

//bool Donate_Dialog::showAgain() const
//{
//    return !again->isChecked();
//}

/* private slots */
void Donate_Dialog::donationClicked()
{
    QString url;
    /*if ( sender()==donate1 ) {
        donate1->setEnabled(false);
        url = QString("https://money.yandex.ru/embed/donate.xml\
?account=410014229313543&quickpay=donate&payment-type-choice=on\
&mobile-payment-type-choice=on&default-sum=300&targets=Donation+for+develop\
&target-visibility=on&project-name=QtVirtManager\
&project-site=http://f1ash.github.io/qt-virt-manager&button-text=01\
&successURL=");
        QDesktopServices::openUrl(QUrl(url));
    } else */

    if ( sender()==donate2 ) {
        donate2->setEnabled(false);
        QClipboard *c = QApplication::clipboard();
        c->setText("5469400029117988", QClipboard::Clipboard);
    }

    /*else if ( sender()==donate3 ) {
        donate3->setEnabled(false);
        QClipboard *c = QApplication::clipboard();
        c->setText(
                "1KnFdQyxjY2PHEamoYjdwYsK1hcC7SzLEE",
                QClipboard::Clipboard);
    } else if ( sender()==donate4 ) {
        donate4->setEnabled(false);
        QClipboard *c = QApplication::clipboard();
        c->setText(
                "bitcoincash:qr7qepftr5nrl0t8p0t8fpg6cq2c9fycmveyv3fsff",
                QClipboard::Clipboard);
    };
    */
}
