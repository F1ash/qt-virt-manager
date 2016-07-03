#ifndef OPEN_FILE_MENU_H
#define OPEN_FILE_MENU_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QIcon>

struct OFILE_TASK {
    QString method;
    QString context;
    QString path;
};

class OpenFileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit OpenFileMenu(
            QWidget *parent = nullptr,
            QString  str = "create",
            QString  src = "Network");

signals:
    void        fileForMethod(const OFILE_TASK&);

private:
    QIcon       icon;
    QAction    *applyAsIs;
    QAction    *editTemplate;
    QAction    *manual;
    OFILE_TASK  task;

private slots:
    void        chooseExample();
    void        chooseManual();
    void        emitParameters(const QString&, const QString&);

};

#endif // OPEN_FILE_MENU_H
