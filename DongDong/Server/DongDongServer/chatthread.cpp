#include "chatthread.h"

ChatThread::ChatThread(QString req, QString id)
{
    str = req;
    srcId = id;
    setAutoDelete(true);
}

void ChatThread::run()
{
    QStringList temp = str.split(",");
    QString target = temp[1];
    QString content = temp[2];

    if(str[1]=='0'){//私聊
        GlobalVariable::clientMutex.lock();
        QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(target);
        if(it==GlobalVariable::clientRequestMap.end()){
            qDebug()<<"没有目标用户";
            GlobalVariable::clientMutex.unlock();
            return;
        }
        GlobalVariable::clientMutex.unlock();
        QString message = "90,"+srcId+","+content;
        it.value()->write(message.toUtf8());
    }
    else{//群聊
        QString message = "91,"+target+","+srcId+","+content;
        GlobalVariable::groupMutex.lock();
        if(GlobalVariable::groupsMembersList.contains(target)){
            for(QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::groupsMembersList[target].begin();it!=GlobalVariable::groupsMembersList[target].end();it++){
                if(it.value()!=NULL && it.key()!=srcId){
                    it.value()->write(message.toUtf8());
                    qDebug()<<it.value()->objectName()<<message;
                }
            }
        }
        else
            qDebug()<<"没有这个群组";
        GlobalVariable::groupMutex.unlock();
    }
}
