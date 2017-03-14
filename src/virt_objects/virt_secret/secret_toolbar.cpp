#include "secret_toolbar.h"

VirtSecretToolBar::VirtSecretToolBar(QWidget *parent) :
    QToolBar(parent)
{
    define_Action = new QAction(this);
    define_Action->setIcon(QIcon::fromTheme("define"));
    define_Action->setToolTip("Define for persistent usage");
    undefine_Action = new QAction(this);
    undefine_Action->setIcon(QIcon::fromTheme("undefine"));
    undefine_Action->setToolTip("Undefine");
    //getXMLDesc_Action = new QAction(this);
    //getXMLDesc_Action->setIcon(QIcon::fromTheme("application-xml"));
    //getXMLDesc_Action->setToolTip("Get XML Description");
    _autoReload = new QPushButton(this);
    _autoReload->setToolTip("AutoReload Secret Overview");
    _autoReload->setIcon(QIcon::fromTheme("view-refresh"));
    _autoReload->setCheckable(true);

    addAction(define_Action);
    addAction(undefine_Action);
    //addSeparator();
    //addAction(getXMLDesc_Action);
    addSeparator();
    autoReload = addWidget(_autoReload);
    addAction(autoReload);

    settings.beginGroup("VirtSecretControl");
    interval = settings.value("UpdateTime", 3).toInt();
    _autoReload->setChecked(settings.value("AutoReload", false).toBool());
    settings.endGroup();

    connect(_autoReload, SIGNAL(toggled(bool)),
            this, SLOT(changeAutoReloadState(bool)));

    connect(this, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(detectTriggeredAction(QAction*)));
}
VirtSecretToolBar::~VirtSecretToolBar()
{
    settings.beginGroup("VirtSecretControl");
    settings.setValue("UpdateTime", interval);
    settings.setValue("AutoReload", _autoReload->isChecked());
    settings.endGroup();
}

/* public slots */
Qt::ToolBarArea VirtSecretToolBar::get_ToolBarArea(int i) const
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
void VirtSecretToolBar::enableAutoReload()
{
    if ( _autoReload->isChecked() )
        timerId = startTimer(interval*1000);
}
void VirtSecretToolBar::stopProcessing()
{
    if ( timerId ) {
        killTimer(timerId);
        timerId = 0;
    }
}
bool VirtSecretToolBar::getAutoReloadState() const
{
    return _autoReload->isChecked();
}

/* private slots */
void VirtSecretToolBar::timerEvent(QTimerEvent *event)
{
    int _timerId = event->timerId();
    //qDebug()<<_timerId<<timerId;
    if ( _timerId && timerId==_timerId ) {
        Act_Param parameters;
        parameters.method = Methods::reloadEntity;
        emit execMethod(parameters);
    };
}
void VirtSecretToolBar::detectTriggeredAction(QAction *action)
{
    Act_Param parameters;
    if        ( action == define_Action ) {
        parameters.method = Methods::defineEntity;
    } else if ( action == undefine_Action ) {
        parameters.method = Methods::undefineEntity;
    //} else if ( action == getXMLDesc_Action ) {
    //    parameters << "getVirtSecretXMLDesc";
    } else return;
    emit execMethod(parameters);
}
void VirtSecretToolBar::changeAutoReloadState(bool state)
{
    if ( state ) enableAutoReload();
    else stopProcessing();
}
