#ifndef LOG_DOC_H
#define LOG_DOC_H

#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QDebug>

#define LOG_SIZE 1048576

class LogDocContent : public QWidget
{
    Q_OBJECT
public:
    explicit LogDocContent(QWidget *parent = 0);
    ~LogDocContent();

private:
    QSize         _size;
    QGridLayout  *docLayout;
    QTextEdit    *Log;

signals:

public slots:
    void appendErrorMsg(QString&);

private slots:

};

#endif // LOG_DOC_H
