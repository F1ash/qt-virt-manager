#include "open_file_menu.h"

OpenFileMenu::OpenFileMenu(QWidget *parent, QString str) :
    QMenu(parent), method(str)
{
    fullMethodName = ( method=="create" ) ? "createVirtNetFromXML" : "defineVirtNetFromXML";
    icon = QIcon::fromTheme( ( method=="create" ) ? "network-create" : "network-define" );
    examples = new QAction(this);
    examples->setText(QString("%1 Virtual Network from example XML").arg(method));
    examples->setIcon(icon);
    custom = new QAction(this);
    custom->setText(QString("%1 Virtual Network from custom XML").arg(method));
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
    emitParameters("Get Example Network XML", EXAMPLES);
}
void OpenFileMenu::customChoised()
{
    emitParameters("Get Custom Network XML", "~");
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
