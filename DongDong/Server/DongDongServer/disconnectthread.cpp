#include "disconnectthread.h"

DisconnectThread::DisconnectThread(QTcpSocket* t)
{
    temp = t;
    setAutoDelete(true);
}

void DisconnectThread::run()
{
    GlobalVariable::clientMutex.lock();
    QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(temp->objectName());
    if(it == GlobalVariable::clientRequestMap.end()){
        if(temp){  //防止没登陆直接关掉
            temp->deleteLater();
            temp = NULL;
        }
    }
    else{
        if(it.value()!=NULL){
            qDebug()<<it.value()<<temp;
            it.value() = NULL;
            qDebug()<<it.value()<<temp;
            GlobalVariable::clientRequestMap.erase(it);
        }
        if(temp){
            temp->deleteLater();
            temp = NULL;
        }
    }
    GlobalVariable::clientMutex.unlock();
    return;
}
