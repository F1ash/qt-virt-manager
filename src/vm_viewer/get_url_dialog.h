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

class URLMenu : public QMenu
{
    Q_OBJECT
public:
    explicit URLMenu(QWidget *parent = nullptr);
    bool             isActivated() const;
private:
    bool             state;
    QAction         *delURL;
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

private:
    QString          url;
    QPushButton     *push;
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
    void             urlMenuRequested(const QPoint&);
    void             urlActivated(QListWidgetItem*);
};

#endif // GET_URL_DIALOG_H
