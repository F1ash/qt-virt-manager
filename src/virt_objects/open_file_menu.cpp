#include "open_file_menu.h"

OpenFileMenu::OpenFileMenu(QWidget *parent, QString str, QString src) :
    QMenu(parent)
{
    task.method = str;
    icon = QIcon::fromTheme( QString("%1").arg(task.method) );
    applyAsIs = new QAction(this);
    applyAsIs->setText(
                QString(
    "%1 Virtual %2 from example XML description as is")
                .arg(task.method).arg(src));
    applyAsIs->setIcon(icon);
    editTemplate = new QAction(this);
    editTemplate->setText(
                QString(
    "%1 Virtual %2 by edit example XML description")
                .arg(task.method).arg(src));
    editTemplate->setIcon(icon);
    manual = new QAction(this);
    manual->setText(QString("%1 Virtual %2 manually")
                    .arg(task.method).arg(src));
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
    task.context.clear();
    task.path.clear();
    if ( sender()==applyAsIs ) {
        task.context.append("AsIs");
    } else if ( sender()==editTemplate ) {
        task.context.append("Edit");
    } else {
        chooseManual();
        return;
    };
    emitParameters("Get Custom Source XML", "~");
}
void OpenFileMenu::chooseManual()
{
    task.context.clear();
    task.path.clear();
    task.context.append("manually");
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
