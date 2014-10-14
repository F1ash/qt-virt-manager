#include "resize_dialog.h"

ResizeDialog::ResizeDialog(QWidget *parent, unsigned long long i) :
    QDialog(parent), _size(i)
{
    sizeLabel = new QLabel("Size:", this);
    size = new QSpinBox(this);
    size->setRange(0, KiB);
    connect(size, SIGNAL(valueChanged(int)), this, SLOT(changeRange(int)));
    currentRange = bytes;
    changeRangeLong(_size);
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    connect(ok, SIGNAL(clicked()), this, SLOT(okClicked()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttons = new QWidget(this);
    buttons->setLayout(buttonLayout);
    commonlayout = new QVBoxLayout();
    commonlayout->addWidget(sizeLabel);
    commonlayout->addWidget(size);
    commonlayout->addWidget(buttons);
    setLayout(commonlayout);
    setResult(0);
}
ResizeDialog::~ResizeDialog()
{
    disconnect(size, SIGNAL(valueChanged(int)), this, SLOT(changeRange(int)));
    disconnect(ok, SIGNAL(clicked()), this, SLOT(okClicked()));
    disconnect(cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    delete sizeLabel;
    sizeLabel = NULL;
    delete size;
    size = NULL;
    delete ok;
    ok = NULL;
    delete cancel;
    cancel = NULL;
    delete buttonLayout;
    buttonLayout = NULL;
    delete buttons;
    buttons = NULL;
    delete commonlayout;
    commonlayout = NULL;
}

/* private slots */
void ResizeDialog::okClicked()
{
    done(1);
}
void ResizeDialog::cancelClicked()
{
    done(0);
}
void ResizeDialog::changeRange(int i)
{
    if ( i==0 && currentRange>bytes ) {
        i = KiB-1;
        currentRange = currentRange/KiB;
    } else if ( i==KiB ) {
        i = 1;
        currentRange = currentRange*KiB;
    };
    _size = i*currentRange;
    changeRangeLong(_size);
}
void ResizeDialog::changeRangeLong(unsigned long long i)
{
    if        ( i >= TiB ) {
        sizeLabel->setText("Size (TiB):");
        currentRange = TiB;
        size->setValue(i/TiB);
    } else if ( i >= GiB ) {
        sizeLabel->setText("Size (GiB):");
        currentRange = GiB;
        size->setValue(i/GiB);
    } else if ( i >= MiB ) {
        sizeLabel->setText("Size (MiB):");
        currentRange = MiB;
        size->setValue(i/MiB);
    } else if ( i >= KiB ) {
        sizeLabel->setText("Size (KiB):");
        currentRange = KiB;
        size->setValue(i/KiB);
    } else {
        sizeLabel->setText("Size (bytes):");
        currentRange = bytes;
        size->setValue(i);
    };
}

/* public slots */
unsigned long long ResizeDialog::getNewSize() const
{
    return size->value()*currentRange;
}
