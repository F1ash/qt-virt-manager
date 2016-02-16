#include "resize_dialog.h"

resizeHelperThread::resizeHelperThread(
        QObject         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _poolName,
        QString          _volName) :
    _VirtThread(parent, connPtrPtr),
    poolName(_poolName), volName(_volName)
{
    _size  = 0;
    result = false;
}
void resizeHelperThread::run()
{
    if ( NULL==ptr_ConnPtr || NULL==*ptr_ConnPtr ) {
        emit ptrIsNull();
        return;
    };
    if ( virConnectRef(*ptr_ConnPtr)<0 ) {
        sendConnErrors();
        return;
    };

    virStoragePoolPtr currStoragePool = virStoragePoolLookupByName(
                *ptr_ConnPtr, poolName.toUtf8().data());
    if ( currStoragePool!=NULL ) {
        virStorageVolPtr storageVol = virStorageVolLookupByName(
                    currStoragePool, volName.toUtf8().data());
        if ( storageVol!=NULL ) {
            virStorageVolInfo info;
            if ( virStorageVolGetInfo(storageVol, &info)+1 ) {
                switch (info.type) {
                case VIR_STORAGE_VOL_FILE:
                case VIR_STORAGE_VOL_NETWORK:
                    _size = info.capacity;
                    result = true;
                    break;
                default:
                    break;
                };
            };
            virStorageVolFree(storageVol);
        };
        virStoragePoolFree(currStoragePool);
    };

    if ( virConnectClose(*ptr_ConnPtr)<0 )
        sendConnErrors();
}


ResizeDialog::ResizeDialog(
        QWidget         *parent,
        virConnectPtr   *connPtrPtr,
        QString          _poolName,
        QString          _volName) :
    QDialog(parent)
{
    setEnabled(false);
    helperThread = new resizeHelperThread(
                this, connPtrPtr,
                _poolName, _volName);
    connect(helperThread, SIGNAL(finished()),
            this, SLOT(setCurrentSize()));
    sizeLabel = new QLabel("Size:", this);
    size = new QSpinBox(this);
    connect(size, SIGNAL(valueChanged(int)),
            this, SLOT(changeRange(int)));
    ok = new QPushButton("Ok", this);
    cancel = new QPushButton("Cancel", this);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(okClicked()));
    connect(cancel, SIGNAL(clicked()),
            this, SLOT(cancelClicked()));
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
    helperThread->start();
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
void ResizeDialog::setCurrentSize()
{
    if ( !helperThread->result ) return;
    setEnabled(true);
    size->setRange(0, KiB);
    currentRange = bytes;
    changeRangeLong(helperThread->_size);
}

/* public slots */
unsigned long long ResizeDialog::getNewSize() const
{
    return size->value()*currentRange;
}
