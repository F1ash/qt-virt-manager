#ifndef TASK_WAREHOUSE_H
#define TASK_WAREHOUSE_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QListWidget>
#include <QThreadPool>
#include <QVBoxLayout>

class TaskWareHouse : public QMainWindow
{
    Q_OBJECT
public:
    explicit TaskWareHouse(QWidget *parent = NULL);

signals:
    void             visibilityChanged(bool);
    void             taskMsg(QString&);

private:
    uint             counter = 0;
    QSettings        settings;
    QListWidget     *taskList;

public slots:
    void             changeVisibility();
    void             saveCurrentState();
    void             stopTaskComputing();
    void             addNewTask(QStringList&);

private slots:
    void             closeEvent(QCloseEvent*);
};

#endif // TASK_WAREHOUSE_H
