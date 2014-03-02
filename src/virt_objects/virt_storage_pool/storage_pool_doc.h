#ifndef STORAGE_POOL_DOC_H
#define STORAGE_POOL_DOC_H

#include <QMainWindow>

class StoragePoolDocContent : public QMainWindow
{
    Q_OBJECT
public:
    explicit StoragePoolDocContent(QWidget *parent = 0);

signals:

public slots:
    void stopProcessing();

};

#endif // STORAGE_POOL_DOC_H
