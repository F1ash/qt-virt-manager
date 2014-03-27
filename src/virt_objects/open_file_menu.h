#ifndef OPEN_FILE_MENU_H
#define OPEN_FILE_MENU_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>

// TODO: make a cross-platform
#define EXAMPLES QString("/usr/share/qt-virt-manager/misc")

class OpenFileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit OpenFileMenu(QWidget *parent = 0, QString str = "create", QString src = "Network");
    ~OpenFileMenu();

signals:
    void fileForMethod(QStringList&);

private:
    QString     method;
    QString     source;
    QString     fullMethodName;
    QIcon       icon;
    QAction    *examples;
    QAction    *custom;
    QAction    *manual;

private slots:
    void exampleChoised();
    void customChoised();
    void manualChoised();
    void emitParameters(const QString&, const QString&);

};

#endif // OPEN_FILE_MENU_H
