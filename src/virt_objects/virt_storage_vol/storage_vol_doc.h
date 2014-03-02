#ifndef STORAGE_VOL_DOC_H
#define STORAGE_VOL_DOC_H

#include <QMainWindow>

class StorageVolDocContent : public QMainWindow
{
    Q_OBJECT
public:
    explicit StorageVolDocContent(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // STORAGE_VOL_DOC_H
