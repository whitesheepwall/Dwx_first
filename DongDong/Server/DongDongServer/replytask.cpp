#include "replytask.h"

ReplyTask::ReplyTask(QObject *parent)
    : QObject{parent}
{

}

void ReplyTask::working()
{
    while(1){
        GlobalVariable::msgQMutex.lock();
        QMap<QString,QString> m = GlobalVariable::msgQ.dequeue();
        GlobalVariable::msgQMutex.unlock();

        GlobalVariable::clientMutex.lock();
        auto it = GlobalVariable::clientRequestMap.find(m.begin().key());
        if(it!=GlobalVariable::clientRequestMap.end()){
            it.value()->write(m.begin().value().toUtf8());
        }
        GlobalVariable::clientMutex.unlock();
    }
}
