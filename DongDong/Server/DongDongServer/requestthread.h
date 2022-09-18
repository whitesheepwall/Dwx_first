#ifndef REQUESTTHREAD_H
#define REQUESTTHREAD_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QBuffer>
#include <QUuid>
#include "sqlconnectionpool.h"

class RequestThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RequestThread(QTcpSocket *t,QString req, QString srcId);

protected:
    void run() override;

public:
    void updateGroupMember(QString, QTcpSocket*);
    QString readLoginAccount(QString);
    QString readRegisterAccount(QString);
    QString readFriend(QString);
    QString queryUser(QString);
    void updateUserInfo(QString);
    void updateUserPic(QString);
    void updateUserRemark(QString);
    void addFriendRequest(QString, QString);
    QString addFriendResult(QString, QString);
    bool addGroup(QString, QString);
    QString readGroup(QString);

private:
    QTcpSocket *temp; //取得客户端socket
    QString str;  //读取请求
    QString srcId;
    QSqlDatabase db;
    QString res;

signals:
    void showLog(QString);
    void onlinePlus();
    void response();
};

#endif // REQUESTTHREAD_H
