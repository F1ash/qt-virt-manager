#ifndef PATH_TO_FILE_H
#define PATH_TO_FILE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include "create_widgets/domain/_changed.h"

class Path_To_File : public _Changed
{
    Q_OBJECT
public:
    explicit Path_To_File(QWidget *parent = 0);

signals:
    //void             dataChanged();

private:
    QPushButton     *button;
    QLineEdit       *path;
    QHBoxLayout     *commonLayout;

public slots:
    QString          getPath() const;
    void             setPath(QString&);
    void             setPlaceholderText(QString&);
    void             clear();

private slots:
    void             searchPath();
};

#endif // PATH_TO_FILE_H
