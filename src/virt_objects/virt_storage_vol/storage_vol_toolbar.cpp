#include "storage_vol_toolbar.h"

/*
 * Volume Actions:
 *
 * virStorageVolCreateXML
 * virStorageVolCreateXMLFrom
 * virStorageVolDelete
 * virStorageVolDownload
 * virStorageVolGetXMLDesc
 * virStorageVolResize
 * virStorageVolUpload
 * virStorageVolWipe
 */

StorageVolToolBar::StorageVolToolBar(QWidget *parent) :
    QToolBar(parent)
{
    create_Action = new QAction(this);
    create_Action->setIcon(QIcon::fromTheme("define"));
    create_Action->setToolTip("New volume");
    create_Menu = new OpenFileMenu(
                this, CREATE_ENTITY, VIRT_STORAGE_VOLUME);
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()),
            this, SLOT(showMenu()));
    delete_Action = new QAction(this);
    delete_Action->setIcon(QIcon::fromTheme("delete"));
    delete_Action->setToolTip("Delete");
    download_Action = new QAction(this);
    download_Action->setIcon(QIcon::fromTheme("download"));
    download_Action->setToolTip("Download");
    resize_Action = new QAction(this);
    resize_Action->setIcon(QIcon::fromTheme("resize"));
    resize_Action->setToolTip("Resize");
    upload_Action = new QAction(this);
    upload_Action->setIcon(QIcon::fromTheme("upload"));
    upload_Action->setToolTip("Upload");
    wipe_Menu = new WipeMenu(this);
    wipe_Action = new QAction(this);
    wipe_Action->setIcon(QIcon::fromTheme("wipe"));
    wipe_Action->setToolTip("Wipe");
    wipe_Action->setMenu(wipe_Menu);
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip("Get XML Description");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload Volume Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(create_Action);
    addAction(delete_Action);
    addSeparator();
    addAction(download_Action);
    addAction(resize_Action);
    addAction(upload_Action);
    addAction(wipe_Action);
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtStorageVolControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(
                settings.value("AutoReload", false)
                .toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(create_Menu, SIGNAL(fileForMethod(const Act_Param&)),
            this, SIGNAL(fileForMethod(const Act_Param&)));
    connect(wipe_Menu, SIGNAL(execMethod(const Act_Param&)),
            this, SIGNAL(execMethod(const Act_Param&)));
    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
}
StorageVolToolBar::~StorageVolToolBar()
{
    settings.beginGroup("VirtStorageVolControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea StorageVolToolBar::get_ToolBarArea(int i) const
{
    Qt::ToolBarArea result;
    switch (i) {
    case 1:
        result = Qt::LeftToolBarArea;
        break;
    case 2:
        result = Qt::RightToolBarArea;
        break;
    case 4:
        result = Qt::TopToolBarArea;
        break;
    case 8:
        result = Qt::BottomToolBarArea;
        break;
    default:
        result = Qt::TopToolBarArea;
        break;
    };
    return result;
}
void StorageVolToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() )
        timerId = startTimer(interval*1000);
}
void StorageVolToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool StorageVolToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void StorageVolToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        Act_Param parameters;
        parameters.method = reloadEntity;
        emit execMethod(parameters);
    };
}
void StorageVolToolBar::showMenu()
{
    QAction *act = static_cast<QAction*>(sender());
    if ( act->menu()->isVisible() ) act->menu()->hide();
    else {
        act->menu()->show();
        act->menu()->move(QCursor::pos());
    };
}
void StorageVolToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    if ( action == upload_Action) {
        parameters.method = uploadVirtStorageVol;
    } else if ( action == delete_Action ) {
        parameters.method = deleteEntity;
    } else if ( action == download_Action ) {
        parameters.method = downloadVirtStorageVol;
    } else if ( action == resize_Action ) {
        parameters.method = resizeVirtStorageVol;
    } else if ( action == wipe_Action ) {
        parameters.method = wipeVirtStorageVol;
        parameters.path = "0";
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtStorageVolXMLDesc";
    } else return;
    emit execMethod(parameters);
}
void StorageVolToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
