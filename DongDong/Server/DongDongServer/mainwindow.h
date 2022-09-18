#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QBuffer>
#include <QUuid>
#include <QThreadPool>
#include "requestthread.h"
#include "disconnectthread.h"
#include "chatthread.h"
#include <QMutex>
#include <QThread>
#include "replytask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void managerLoginSuccess();
    void openServer();
    void closeServer();
    void initGroup();
    void updateGroupMember(QString,QTcpSocket*);
    void processConnections();
    //void forwardText(QString,QString);
    void clientDisconnected();
    void clientRequestDisconnected();
    void updateOnlineNum();
    void processAccountConnection();
    void analysisRequest();

    //处理请求
    //QString readFriend(QString);  //读取用户好友列表
    //QString readGroup(QString);  //读取用户群组列表
    //QString readLoginAccount(QString);  //验证用户登录
    //QString readRegisterAccount(QString);  //验证用户注册
    //QString queryUser(QString);   //加好友查找用户
    //void updateUserInfo(QString);  //更新用户信息
    //void updateUserPic(QString);  //更新用户头像
    //void updateUserRemark(QString);  //更新用户备注
    //void addFriendRequest(QString,QString);  //加好友请求
    //QString addFriendResult(QString,QString); //加好友结果
    //bool addGroup(QString,QString);

    void replyToClient();

private:
    Ui::MainWindow *ui;

    QThreadPool *pool;
    //QByteArray defaultHeadIcon;
    //QByteArray defaultGroupIcon;
    int onlineNum;
    QTcpServer *verifyServer;

};
#endif // MAINWINDOW_H
