#include "mainwindow.h"
#include "loginandregister.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    LoginAndRegister *lar = new LoginAndRegister;
    lar->show();

    //客户端发起登录请求
    QObject::connect(lar,&LoginAndRegister::loginInfo, &w, &MainWindow::onLogin);

    //客户端发起注册请求
    QObject::connect(lar,&LoginAndRegister::registerInfo, &w, &MainWindow::onRegister);

    //服务器返回登录结果
    QObject::connect(&w,&MainWindow::loginResult, lar, &LoginAndRegister::loginResult);

    //服务器返回注册结果
    QObject::connect(&w,&MainWindow::registerResult, lar, &LoginAndRegister::registerResult);

    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection")){
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("DongDong.db");
    }
    if(!db.open()){
        qDebug()<<"数据库打开失败";
    }

    QSqlQuery query;
    if(!query.exec("PRAGMA foreign_keys = ON;")){
        qDebug()<<"开启外键失败";
    }
    if(!query.exec("create table if not exists conversation("
                   "targetId vchar(40),"
                   "myId vchar(15),"
                   "type int)"
                   )){

        qDebug()<<"会话表创建失败"<<query.lastError();
    }
    if(!query.exec("create table if not exists friendmessage("
                   "messageId vchar(40) primary key, "
                   "sequence int AUTO_INCREMENT, "
                   "senderId vchar(15), "
                   "receiverId vchar(15), "
                   "content vchar(5000), "
                   "time timestamp)"
                   )){

        qDebug()<<"消息表创建失败"<<query.lastError();
    }
    if(!query.exec("create table if not exists groupmessage("
                   "messageId vchar(40) primary key, "
                   "groupId vchar(40),"
                   "sequence int AUTO_INCREMENT, "
                   "senderId vchar(15), "
                   "content vchar(5000), "
                   "time timestamp)"
                   )){

        qDebug()<<"群组消息表创建失败"<<query.lastError();
    }

    return a.exec();
}
