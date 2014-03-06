#include "open_file_menu.h"

OpenFileMenu::OpenFileMenu(QWidget *parent, QString str, QString src) :
    QMenu(parent), method(str), source(src)
{
    fullMethodName = QString("%1_FromXML").arg(method.toLower());
    icon = QIcon::fromTheme( QString("%1-%2").arg(src.toLower()).arg(method.toLower()) );
    examples = new QAction(this);
    examples->setText(QString("%1 Virtual %2 from example XML").arg(method).arg(src));
    examples->setIcon(icon);
    custom = new QAction(this);
    custom->setText(QString("%1 Virtual %2 from custom XML").arg(method).arg(src));
    custom->setIcon(icon);
    connect(examples, SIGNAL(triggered()), this, SLOT(exampleChoised()));
    connect(custom, SIGNAL(triggered()), this, SLOT(customChoised()));
    addAction(examples);
    addAction(custom);
}
OpenFileMenu::~OpenFileMenu()
{
    disconnect(examples, SIGNAL(triggered()), this, SLOT(exampleChoised()));
    disconnect(custom, SIGNAL(triggered()), this, SLOT(customChoised()));
    delete examples;
    examples = 0;
    delete custom;
    custom = 0;
}
void OpenFileMenu::exampleChoised()
{
    QString path = QString("%1/%2").arg(EXAMPLES).arg(source.toLower());
    emitParameters("Get Example Source XML", path);
}
void OpenFileMenu::customChoised()
{
    emitParameters("Get Custom Source XML", "~");
}
void OpenFileMenu::emitParameters(const QString &title, const QString &dirPath)
{
    QString path;
    path = QFileDialog::getOpenFileName(this, title, dirPath);
    if ( !path.isEmpty() ) {
        QStringList parameters;
        parameters << fullMethodName << path;
        emit fileForMethod(parameters);
    };
}
