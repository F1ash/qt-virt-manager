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
    create_Action->setIcon(QIcon::fromTheme("storageVol-create"));
    create_Action->setToolTip("Create");
    create_Menu = new OpenFileMenu(this, "create", "StorageVol");
    create_Action->setMenu(create_Menu);
    connect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));
    delete_Action = new QAction(this);
    delete_Action->setIcon(QIcon::fromTheme("storageVol-stop"));
    delete_Action->setToolTip("Delete");
    download_Action = new QAction(this);
    download_Action->setIcon(QIcon::fromTheme("storageVol-autostart"));
    download_Action->setToolTip("Download");
    resize_Action = new QAction(this);
    resize_Action->setIcon(QIcon::fromTheme("storageVol-resize"));
    resize_Action->setToolTip("Resize");
    upload_Action = new QAction(this);
    upload_Action->setIcon(QIcon::fromTheme("storageVol-start"));
    upload_Action->setToolTip("Upload");
    getXMLDesc_Action = new QAction(this);
    getXMLDesc_Action->setIcon(QIcon::fromTheme("storageVol-xml"));
    getXMLDesc_Action->setToolTip("Get XML Description");
    stopOverview_Action = new QAction(this);
    stopOverview_Action->setIcon(QIcon::fromTheme("overview-stop"));
    stopOverview_Action->setToolTip("Close Pool Overview");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload Pool Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(create_Action);
    addAction(delete_Action);
    addAction(download_Action);
    addAction(resize_Action);
    addAction(upload_Action);
    addSeparator();
    addAction(getXMLDesc_Action);
    addSeparator();
    addAction(stopOverview_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtStorageVolControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(settings.value("AutoReload", false).toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)), this, SLOT(changeAutoReloadState(bool)));

    connect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    connect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));
}
StorageVolToolBar::~StorageVolToolBar()
{
    settings.beginGroup("VirtStorageVolControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
    disconnect(_autoReload, SIGNAL(toggled(bool)), this, SLOT(changeAutoReloadState(bool)));
    disconnect(create_Menu, SIGNAL(fileForMethod(QStringList&)), this, SLOT(repeatParameters(QStringList&)));
    disconnect(this, SIGNAL(actionTriggered(QAction*)), this, SLOT(detectTriggerredAction(QAction*)));

    disconnect(create_Action, SIGNAL(triggered()), this, SLOT(showMenu()));

    delete create_Action;
    create_Action = 0;
    delete create_Menu;
    create_Menu = 0;
    delete delete_Action;
    delete_Action = 0;
    delete download_Action;
    download_Action = 0;
    delete resize_Action;
    resize_Action = 0;
    delete upload_Action;
    upload_Action = 0;
    delete getXMLDesc_Action;
    getXMLDesc_Action = 0;
    delete stopOverview_Action;
    stopOverview_Action = 0;
    delete _autoReload;
    _autoReload = 0;
    delete autoReload;
    autoReload = 0;
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
    if ( _autoReload->isChecked() ) timerId = startTimer(interval*1000);
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
    if ( _timerId && timerId==_timerId && isVisible() ) {
        QStringList parameters;
        parameters << "reloadVirtStoragePool";
        emit execMethod(parameters);
    };
}
void StorageVolToolBar::repeatParameters(QStringList &p)
{
    emit fileForMethod(p);
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
void StorageVolToolBar::detectTriggerredAction(QAction *action)
{
    QStringList parameters;
    if ( action == upload_Action) {
        parameters << "uploadVirtStorageVol";
    } else if ( action == delete_Action ) {
        parameters << "deleteVirtStorageVol";
    } else if ( action == resize_Action ) {
        parameters << "resizeVirtStorageVol";
    } else if ( action == download_Action ) {
        parameters << "downloadVirtStorageVol";
    } else if ( action == getXMLDesc_Action ) {
        parameters << "getVirtStorageVolXMLDesc";
    } else if ( action == stopOverview_Action ) {
        parameters << "stopOverViewVirtStoragePool";
    } else return;
    emit execMethod(parameters);
}
void StorageVolToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
