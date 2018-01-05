#ifndef GET_URL_DIALOG_H
#define GET_URL_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSettings>
#include <QPoint>
#include <QMenu>
#include <QCloseEvent>

enum Menu_Act {
    NONE,
    DELETE_ITEM,
    CLEAR_LIST
};

class URLMenu : public QMenu
{
    Q_OBJECT
public:
    explicit URLMenu(QWidget *parent = nullptr);
    Menu_Act         getState() const;
private:
    Menu_Act         state = NONE;
    QAction         *delURL;
    QAction         *clearList;
private slots:
    void             actionTriggered(QAction*);
};

class GetURLDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GetURLDialog(QWidget *parent = nullptr);
    ~GetURLDialog();
    QString          getURL() const;
    void             saveToHistory(const QString&);

private:
    QString          url;
    QPushButton     *push, *info;
    QLineEdit       *urlEdit;
    QHBoxLayout     *urlLayout;
    QWidget         *urlWdg;
    QListWidget     *urlList;
    QVBoxLayout     *commonLayout;

    QSettings        settings;
    bool             isUnique(const QString&) const;

private slots:
    void             closeEvent(QCloseEvent*);
    void             saveUniqueURL();
    void             showInfo();
    void             urlMenuRequested(const QPoint&);
    void             urlActivated(QListWidgetItem*);
};

#endif // GET_URL_DIALOG_H
