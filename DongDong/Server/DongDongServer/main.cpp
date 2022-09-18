#include "login.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

void createDb();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    createDb();
    MainWindow w;
    Login *l = new Login; 
    QObject::connect(l,&Login::managerLoginSuccess, &w, &MainWindow::managerLoginSuccess);
    l->show();

    return a.exec();
}

void createDb()
{
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
        return;
    }

    QSqlQuery query;
    if(!query.exec("PRAGMA foreign_keys = ON;")){
        qDebug()<<"开启外键失败";
        return;
    }
    if(!query.exec("create table if not exists account("
                   "userId vchar(15) primary key, "
                   "nickname vchar(20), "
                   "sex vchar(5), "
                   "signature vchar(50), "
                   "headIcon blob,"
                   "password vchar(15))"
                   )){

        qDebug()<<"账号表创建失败";
        return;
    }

    if(!query.exec("create table if not exists friend("
                   "friendId vchar(15), "
                   "userId vchar(15), "
                   "nickname vchar(20), "
                   "sex vchar(5), "
                   "signature vchar(50), "
                   "remark vchar(20) ,"
                   "CONSTRAINT fk_friendId FOREIGN KEY(friendId) REFERENCES account(userId),"
                   "CONSTRAINT fk_userId FOREIGN KEY(userId) REFERENCES account(userId))"
                   )){

        qDebug()<<"好友表创建失败"<<query.lastError();
        return;
    }

    if(!query.exec("create table if not exists groups("
                   "groupId vchar(40) primary key,"
                   "groupName vchar(20),"
                   "groupIcon blob,"
                   "numOfMember int)"
                   )){

        qDebug()<<"群组信息表创建失败"<<query.lastError();
        return;
    }

    if(!query.exec("create table if not exists userGroup("
                   "userId vchar(15), "
                   "groupId vchar(40),"
                   "CONSTRAINT fk_userId FOREIGN KEY(userId) REFERENCES account(userId),"
                   "CONSTRAINT fk_groupId FOREIGN KEY(groupId) REFERENCES groups(groupId))"
                   )){

        qDebug()<<"用户所属群组表创建失败"<<query.lastError();
        return;
    }
}
