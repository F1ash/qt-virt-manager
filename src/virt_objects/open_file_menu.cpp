#include "open_file_menu.h"

OpenFileMenu::OpenFileMenu(
        QWidget *parent, Actions _act, VIRT_ENTITY _e) :
    QMenu(parent)
{
    act = _act;
    QString __act;
    switch (act) {
    case Actions::CREATE_ENTITY:
        __act = "create";
        break;
    case Actions::DEFINE_ENTITY:
        __act = "define";
        break;
    case Actions::_NONE_ACTION:
    default:
        break;
    };
    icon = QIcon::fromTheme(__act);
    applyAsIs = new QAction(this);
    TO_STRING = new _TO_STRING;
    applyAsIs->setText(
                QString(
    tr("%1 Virtual %2 from example XML description as is"))
                .arg(TO_STRING->enumToActionString(_act))
                .arg(TO_STRING->enumToEntityString(_e)));
    applyAsIs->setIcon(icon);
    editTemplate = new QAction(this);
    editTemplate->setText(
                QString(
    tr("%1 Virtual %2 by edit example XML description"))
                .arg(TO_STRING->enumToActionString(_act))
                .arg(TO_STRING->enumToEntityString(_e)));
    editTemplate->setIcon(icon);
    manual = new QAction(this);
    manual->setText(
                QString(tr("%1 Virtual %2 manually"))
                .arg(TO_STRING->enumToActionString(_act))
                .arg(TO_STRING->enumToEntityString(_e)));
    manual->setIcon(icon);
    connect(applyAsIs, SIGNAL(triggered()),
            this, SLOT(chooseExample()));
    connect(editTemplate, SIGNAL(triggered()),
            this, SLOT(chooseExample()));
    connect(manual, SIGNAL(triggered()),
            this, SLOT(chooseManual()));
    addAction(applyAsIs);
    addAction(editTemplate);
    addSeparator();
    addAction(manual);
}

void OpenFileMenu::chooseExample()
{
    task.clear();
    if ( sender()==applyAsIs ) {
        task.context = HOW_TO_DO::DO_AsIs;
    } else if ( sender()==editTemplate ) {
        task.context = HOW_TO_DO::DO_Edit;
    } else {
        chooseManual();
        return;
    };
    emitParameters(tr("Get Custom Source XML"), "~");
}
void OpenFileMenu::chooseManual()
{
    task.clear();
    task.act = act;
    task.context = HOW_TO_DO::DO_Manually;
    emit fileForMethod(task);
}
void OpenFileMenu::emitParameters(const QString &title, const QString &dirPath)
{
    QString path;
    path = QFileDialog::getOpenFileName(this, title, dirPath);
    if ( !path.isEmpty() ) {
        task.act = act;
        task.path.append( path );
        emit fileForMethod(task);
    };
}
