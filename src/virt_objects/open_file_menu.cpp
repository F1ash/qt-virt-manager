#include "open_file_menu.h"

OpenFileMenu::OpenFileMenu(
        QWidget *parent, Actions act, VIRT_ENTITY _e) :
    QMenu(parent)
{
    task.act = act;
    icon = QIcon::fromTheme(
                QString("%1").arg(enumToActionString(act)) );
    applyAsIs = new QAction(this);
    applyAsIs->setText(
                QString(
    "%1 Virtual %2 from example XML description as is")
                .arg(enumToActionString(act))
                .arg(enumToEntityString(_e)));
    applyAsIs->setIcon(icon);
    editTemplate = new QAction(this);
    editTemplate->setText(
                QString(
    "%1 Virtual %2 by edit example XML description")
                .arg(enumToActionString(act))
                .arg(enumToEntityString(_e)));
    editTemplate->setIcon(icon);
    manual = new QAction(this);
    manual->setText(
                QString("%1 Virtual %2 manually")
                .arg(enumToActionString(act))
                .arg(enumToEntityString(_e)));
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
        task.context = DO_AsIs;
    } else if ( sender()==editTemplate ) {
        task.context = DO_Edit;
    } else {
        chooseManual();
        return;
    };
    emitParameters("Get Custom Source XML", "~");
}
void OpenFileMenu::chooseManual()
{
    task.clear();
    task.context = DO_Manually;
    emit fileForMethod(task);
}
void OpenFileMenu::emitParameters(const QString &title, const QString &dirPath)
{
    QString path;
    path = QFileDialog::getOpenFileName(this, title, dirPath);
    if ( !path.isEmpty() ) {
        task.path.append( path );
        emit fileForMethod(task);
    };
}
