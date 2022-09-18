#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QList>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QMap>
#include <QHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QUuid>

#include "loginandregister.h"
#include "config.h"
#include "conversation.h"
#include "friendwidget.h"
#include "information.h"
#include "setinfomation.h"
#include "searchwidget.h"
#include "queryuserwidget.h"
#include "setremarkwidget.h"
#include "friendrequestwidget.h"
#include "friendrequestwindow.h"
#include "mymessage.h"
#include "friendmessage.h"
#include "mychatwidget.h"
#include "addgroup.h"
#include "user.h"
#include "groupwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //初始化页面
    void initScene();
    void gotoPage(int);

    //发送请求
    void getAnswerFromServer();
    void onLogin(QString,QString);
    void onRegister(QString,QString);
    void getMyInfo(QString);
    void queryMyFriend();
    void queryUser();
    void changeInfo(QString,QString,QString);
    void changePic(QString,QPixmap);
    void changeRemarkRequest(QString);
    void addFriendRequest();
    void sendMessageRequest();
    void sendMessage();
    void queryMyGroup();

    //功能函数
    void changeChatInfo(QString,QString,QPixmap,int);
    void updateConversationChoosedState();
    void updateFriendChoosedState(int);

    void searchFriend();
    void restoreSearchFriend();
    void addConversationWidget(QString, QString, QPixmap, QString, int, QString content = "", bool showWidget = false);
    void addGroupWidget(QString,QString,int,QPixmap,QVector<QString>&);
    void addFriendWidget(QString, QString, QString, QString, QString, QPixmap);
    void friendRequestAccept(QString);
    void showUserInfoPanel(QString,QString,QString,QString,QString,QPixmap);
    void setRemark();
    void addFriendOrSendMessage();
    void setRecord(MyChatWidget*,QString,int);
    void addRecord(MyChatWidget*,QString,QString,QString,int,int);
    bool writeConversationToDB(QString,int);
    void showConversation();
    void addGroup();

    void getFriend(QString); //获取服务器返回的好友
    void getGroup(QString);  //获取服务器返回的群组
    void getUser(QString);  //获取服务器返回的用户
    void friendRequest(QString); //获取服务器发来的好友申请
    void recvMessage(QString);
    void AddGroupAccept(QString);//获取服务器发来的加成功的群组

private:
    Ui::MainWindow *ui;

    //个人信息
    QString id;
    QString nickname;
    QString sex;
    QString signature;
    QPixmap headIcon;

    //好友变量
    QPixmap friendPic;
    QString currFriendId;

    //系统变量
    int type;  //0代表私聊，1代表群聊
    Conversation *preConversationWidget = NULL;  //上一个选中的会话框
    FriendWidget *preFriendWidget = NULL;  //上一个选中的好友框
    GroupWidget *preGroupWidget = NULL;  //上一个选中的好友框
    MyChatWidget *preRecordWidget = NULL;
    QTcpSocket *infoSocket = NULL;
    QMap<QString,Conversation*> conversationList;
    QMap<QString,GroupWidget*> groupList;
    QMap<QString,FriendWidget*> friendList;
    QMap<QString,MyChatWidget*> chatRecordList;
    QList<FriendWidget*> searchFriendList;
    QList<GroupWidget*> searchGroupList;
    QHash<QString,User*> relativeUserList;

    //界面
    QLabel *noThatUser;
    QueryUserWidget *queried_user; //加好友面板的好友条目
    SetInfomation *s;
    SetRemarkWidget *rw;

signals:
    void loginResult(bool);
    void registerResult(bool);
    void getFriendFinished();
    void getGroupFinished();
};
#endif // MAINWINDOW_H
