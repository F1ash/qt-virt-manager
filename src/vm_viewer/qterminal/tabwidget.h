
#ifndef TAB_WIDGET
#define TAB_WIDGET

#include <QTabWidget>
#include <QMap>

#include "properties.h"

class TermWidgetHolder;
class QAction;
class QActionGroup;


class TabWidget : public QTabWidget
{
Q_OBJECT
public:
    TabWidget(QWidget* parent = 0,
              QString  _name = QString());

    TermWidgetHolder * terminalHolder();

public slots:
    int addNewTab(const QString& shell_program = QString());
    void removeTab(int);
    void removeCurrentTab();
    void removeFinished();
    void renameSession();
    void setWorkDirectory(const QString&);

    void copySelection();
    void pasteClipboard();
    void pasteSelection();
    void zoomIn();
    void zoomOut();
    void zoomReset();

    void changeTabPosition(QAction *);
    void changeScrollPosition(QAction *);
    void propertiesChanged();

    void clearActiveTerminal();

signals:
    void closeTabNotification();

private:
    const QString tabName;
    QString work_dir;
};

#endif
