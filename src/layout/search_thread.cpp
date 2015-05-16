#include "search_thread.h"

SearchThread::SearchThread(QObject *parent) :
    QThread(parent)
{

}
SearchThread::~SearchThread()
{
    wait(30000);
}

void SearchThread::run()
{

}
