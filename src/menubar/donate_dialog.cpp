#include "donate_dialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>
#include <QClipboard>

Donate_Dialog::Donate_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Donate");
    donate1 = new Click_Label(this);
    donate1->setToolTip("Donate by Yandex.Money");
    donate1->setOpenExternalLinks(true);
    donate1->setPixmap(QIcon::fromTheme("yandex_money").pixmap(128));
    donate2 = new Click_Label(this);
    donate2->setToolTip("Donate to VISA QIWI Wallet\nCopy card number to Clipboard");
    donate2->setOpenExternalLinks(true);
    donate2->setPixmap(QIcon::fromTheme("qiwi_wallet_visa").pixmap(128));
    donateLayout = new QGridLayout(this);
    donateLayout->addWidget(donate1, 0, 0, Qt::AlignCenter);
    donateLayout->addWidget(donate2, 0, 1, Qt::AlignCenter);
    donateWdg = new QWidget(this);
    donateWdg->setLayout(donateLayout);
    again = new QCheckBox("don't show again", this);
    again->hide();
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(donateWdg);
    commonLayout->addWidget(again);
    setLayout(commonLayout);
    connect(donate1, SIGNAL(released()),
            this, SLOT(donationClicked()));
    connect(donate2, SIGNAL(released()),
            this, SLOT(donationClicked()));
}

bool Donate_Dialog::showAgain() const
{
    return !again->isChecked();
}

/* private slots */
void Donate_Dialog::donationClicked()
{
    QString url;
    if ( sender()==donate1 ) {
        donate1->setEnabled(false);
        url = QString("https://money.yandex.ru/embed/donate.xml\
?account=410014229313543&quickpay=donate&payment-type-choice=on\
&mobile-payment-type-choice=on&default-sum=300&targets=Donation+for+develop\
&target-visibility=on&project-name=QtVirtManager\
&project-site=http://f1ash.github.io/qt-virt-manager&button-text=01\
&successURL=");
    } else if ( sender()==donate2 ) {
        donate2->setEnabled(false);
        QClipboard *c = QApplication::clipboard();
        c->setText("4890494460088297", QClipboard::Clipboard);
    };
    if ( !url.isEmpty() )
            QDesktopServices::openUrl(QUrl(url));
}
