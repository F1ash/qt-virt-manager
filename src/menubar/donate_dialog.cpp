#include "donate_dialog.h"
#include <QDesktopServices>
#include <QUrl>

Donate_Dialog::Donate_Dialog(QWidget *parent) :
    QDialog(parent)
{
    donate1 = new Click_Label(this);
    donate1->setToolTip("Donate by Yandex.Money");
    donate1->setOpenExternalLinks(true);
    donate1->setPixmap(QIcon::fromTheme("yandex_money").pixmap(128));
    donateLayout = new QGridLayout(this);
    donateLayout->addWidget(donate1, 0, 0, Qt::AlignCenter);
    donateWdg = new QWidget(this);
    donateWdg->setLayout(donateLayout);
    again = new QCheckBox("don't show again", this);
    again->hide();
    commonLayout = new QVBoxLayout(this);
    commonLayout->addWidget(donateWdg);
    commonLayout->addWidget(again);
    setLayout(commonLayout);
    connect(donate1, SIGNAL(released()),
            this, SLOT(donation1Clicked()));
}

bool Donate_Dialog::showAgain() const
{
    return !again->isChecked();
}

/* private slots */
void Donate_Dialog::donation1Clicked()
{
    donate1->setEnabled(false);
    QString url1 = QString("https://money.yandex.ru/embed/donate.xml\
?account=410014229313543&quickpay=donate&payment-type-choice=on\
&mobile-payment-type-choice=on&default-sum=300&targets=Donation+for+develop\
&target-visibility=on&project-name=QtVirtManager\
&project-site=http://f1ash.github.io/qt-virt-manager&button-text=01\
&successURL=");
    QDesktopServices::openUrl(QUrl(url1));
}
