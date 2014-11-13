#ifndef TASK_BAR_H
#define TASK_BAR_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QListWidget>
#include <QVBoxLayout>

class TaskBar : public QMainWindow
{
    Q_OBJECT
public:
    explicit TaskBar(QWidget *parent = NULL);

signals:
    void             visibilityChanged(bool);

private:
    QSettings        settings;
    QListWidget     *taskList;

public slots:
    void             changeVisibility();
    void             saveCurrentState();
    void             stopTaskComputing();

private slots:
    void             closeEvent(QCloseEvent*);
};

#endif // TASK_BAR_H
