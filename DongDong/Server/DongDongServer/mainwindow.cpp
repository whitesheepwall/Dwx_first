#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globalvariable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("咚咚服务器");
    ui->closeServer->setEnabled(false);
    ui->stateBtn->setFixedSize(20,20);
    ui->stateBtn->setStyleSheet("QPushButton{border-radius:10px;"
                                "background-color:red};");
    ui->stateLabel->setFixedSize(100,20);
    onlineNum = 0;
    ui->onlineNum->hide();

    verifyServer = new QTcpServer(this);
    connect(verifyServer,&QTcpServer::newConnection, this, &MainWindow::processAccountConnection);

    connect(ui->openServer, &QPushButton::clicked, this, &MainWindow::openServer);
    connect(ui->closeServer, &QPushButton::clicked, this, &MainWindow::closeServer);

    connect(ui->actionquit, &QAction::triggered,[=](){
        closeServer();
        this->close();
    });

    pool = new QThreadPool;
    pool->setMaxThreadCount(1024);
    GlobalVariable::initVariable();
}

MainWindow::~MainWindow()
{
    delete ui;
    pool->waitForDone(-1);
    if(pool){
        pool->deleteLater();
        pool = NULL;
    }
}

void MainWindow::managerLoginSuccess()
{
    this->show();
}

void MainWindow::openServer()  //服务器打开
{ 
    ui->openServer->setEnabled(false);
    ui->closeServer->setEnabled(true);
    updateOnlineNum();
    ui->onlineNum->show();

    verifyServer->listen(QHostAddress::Any, 8887);

    if(verifyServer->isListening()){
        ui->messageEdit->append("服务器已经启动");
        ui->stateBtn->setStyleSheet("QPushButton{background-color:green;"
                                    "border-radius:10px};");
        ui->stateLabel->setText("已连接");

        initGroup();  //初始化群组成员结构
    }
    else{
        ui->messageEdit->append("服务器启动失败");
        return;
    }  
}

void MainWindow::closeServer()  //服务器关闭
{
    QString str = QString("服务器已关闭");
    verifyServer->close();
    //清空聊天连接
    GlobalVariable::clientMutex.lock();
    for(QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.begin();it!=GlobalVariable::clientRequestMap.end();it++)
    {
        if(it.value()){
            it.value()->deleteLater();
            it.value() = NULL;
        }
    }
    GlobalVariable::clientRequestMap.clear();
    GlobalVariable::clientMutex.unlock();

    ui->openServer->setEnabled(true);
    ui->stateBtn->setStyleSheet("QPushButton{background-color:red;"
                                "border-radius:10px;};");
    ui->stateLabel->setText("未连接");
    ui->closeServer->setEnabled(false);
    ui->messageEdit->append(str);
    onlineNum = 0;
    ui->onlineNum->hide();
}

void MainWindow::initGroup()
{
    QSqlQuery query1;
    QSqlQuery query2;
    QString str1 = QString("select groupId from groups");
    if(!query1.exec(str1)){
        qDebug()<<"1初始化群组失败";
        return;
    }
    QString str2;

    GlobalVariable::groupMutex.lock();
    while(query1.next()){
        str2 = QString("select userId from userGroup where groupId='%1'").arg(query1.value(0).toString());
        if(!query2.exec(str2)){
            qDebug()<<"2初始化群组失败";
            return;
        }
        QMap<QString,QTcpSocket*> temp;
        while(query2.next()){
            temp.insert(query2.value(0).toString(),NULL);
        }
        GlobalVariable::groupsMembersList.insert(query1.value(0).toString(),temp);
    }
    GlobalVariable::groupMutex.unlock();
}

void MainWindow::clientRequestDisconnected()  //用户主动断开连接
{
    QTcpSocket *temp = qobject_cast<QTcpSocket*>(sender());
    ui->messageEdit->append("断开连接");
    qDebug()<<temp<<"请求断开连接";

    disconnect(temp,&QTcpSocket::disconnected,this,&MainWindow::clientRequestDisconnected);
    disconnect(temp,&QTcpSocket::readyRead,this,&MainWindow::analysisRequest);

    DisconnectThread *task = new DisconnectThread(temp);
    pool->start(task);

    --onlineNum;
    updateOnlineNum();
}

void MainWindow::updateOnlineNum()  //更新在线人数
{
    QString str = QString("当前在线人数：%1").arg(this->onlineNum);
    ui->onlineNum->setText(str);
}

void MainWindow::processAccountConnection() //取得验证登录连接
{
    while(verifyServer->hasPendingConnections())
    {
        QTcpSocket *temp = verifyServer->nextPendingConnection();
        connect(temp, &QTcpSocket::readyRead, this, &MainWindow::analysisRequest);
        connect(temp, &QTcpSocket::disconnected, this, &MainWindow::clientRequestDisconnected);
    }
}

void MainWindow::analysisRequest()  //解析客户端的请求
{
    QTcpSocket *temp = qobject_cast<QTcpSocket*>(sender()); //取得客户端socket
    QString str = temp->readAll();  //读取请求
    QString srcId = temp->objectName();

    qDebug()<<str;
    if(str[0]=='0' || str[0]=='1' || str[0]=='2' || str[0]=='3' || str[0]=='4' || str[0]=='5' || str[0]=='6' || str[0]=='7' || str[0]=='8' || str[0]=='G' || str=="RG")
    {
        RequestThread *task = new RequestThread(temp,str,srcId);
        connect(task,&RequestThread::response,this,&MainWindow::replyToClient);
        connect(task,&RequestThread::showLog,[=](QString s){
            ui->messageEdit->append(s);
        });
        connect(task,&RequestThread::onlinePlus,[=](){
            ++onlineNum;
            updateOnlineNum();
        });
        pool->start(task);
    }   
    else if(str[0]=='9'){   //聊天请求
        //ChatThread *task = new ChatThread(str,srcId);
        //pool->start(task);

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
    else{
        //qDebug()<<str;
    }
}

void MainWindow::replyToClient()
{
    //GlobalVariable::msgQMutex.lock();
    //GlobalVariable::msgQ.emplace_back(res);
    //GlobalVariable::msgQMutex.unlock();
    //通知回复线程有任务了
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





















