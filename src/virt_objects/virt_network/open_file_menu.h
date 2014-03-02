#ifndef OPEN_FILE_MENU_H
#define OPEN_FILE_MENU_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>

#define EXAMPLES QString("/usr/share/sandbox-runner-data/misc/virt-sandbox-runner/network")

class OpenFileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit OpenFileMenu(QWidget *parent = 0, QString str = "create");
    ~OpenFileMenu();

signals:
    void fileForMethod(QStringList&);

private:
    QString     method;
    QString     fullMethodName;
    QIcon       icon;
    QAction    *examples;
    QAction    *custom;

private slots:
    void exampleChoised();
    void customChoised();
    void emitParameters(const QString&, const QString&);

};

#endif // OPEN_FILE_MENU_H
