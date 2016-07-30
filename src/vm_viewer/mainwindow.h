#ifndef VIEWER_MAINWINDOW_H
#define VIEWER_MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "get_url_dialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(
            QWidget *parent = nullptr,
            bool     getURL = true);
    ~MainWindow();

signals:
    void                urlGiven(const QString&);

private:
    QSettings           settings;

private slots:
    void                showGetURLDialog();
public slots:
    void                connectToURL(const QString&);
};

#endif // VIEWER_MAINWINDOW_H
