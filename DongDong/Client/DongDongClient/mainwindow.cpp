#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setAttribute(Qt::WA_DeleteOnClose);

    infoSocket = new QTcpSocket(this);
    connect(infoSocket,&QTcpSocket::connected,[=](){
        qDebug()<<"set information link success";
    });
    connect(infoSocket,&QTcpSocket::readyRead,this, &MainWindow::getAnswerFromServer);

    infoSocket->connectToHost("127.0.0.1", 8887);  //用来设置个人信息的连接
}

MainWindow::~MainWindow()
{
    delete ui;
    if(s!=NULL){  //个人信息设置页面删除
        delete s;
        s = NULL;
    }
    if(queried_user!=NULL){
        delete queried_user;
        queried_user = NULL;
    }
    if(noThatUser!=NULL){
        delete noThatUser;
        noThatUser = NULL;
    }
    if(rw!=NULL){
        delete rw;
        rw = NULL;
    }
}

/*if(1)
    {
        //将照片以二进制流的方式存到数据库
        QPixmap pixmap("./image/test.png");
        QByteArray byteArray = QByteArray();
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer,"png",0);
        QString str = QString("22");//图片ID
        //插入数据库 TEST_PIC为数据库表明  CONTENT为存储照片字段 为Blob类型
        QString strQSL = QString("insert into TEST_PIC(ID,CONTENT) values('%1',?)").arg(str);
        QSqlQuery query;
        query.prepare(strQSL);
        query.addBindValue(byteArray);
        query.exec();
    }
    if(1)
    {
        //从数据库读取照片
        QByteArray byteText;
        QString strsql = QString("select CONTENT,ID from TEST_PIC");
        QSqlQuery query;
        query.exec(strsql);
        while (query.next()) {
            byteText = query.value(0).toByteArray();
            QString id = query.value(1).toString();
            QPixmap pix;
            pix.loadFromData(byteText,"png");
            pix.save(QString("./test2.png"));//保存从数据库读取的照片到本地
        }
    }*/
void MainWindow::initScene()
{
    ui->stackedWidget->setCurrentIndex(0);  //固定到聊天界面

    QPixmap pix;
    pix.load(MYCHAT_1);
    ui->myChatBtn->setIcon(QPixmap(pix));

    pix.load(MYFRIEND_0);
    ui->myFriendBtn->setIcon(QPixmap(pix));

    pix.load(EXPRESSION);
    ui->expressionBtn->setIcon(QPixmap(pix));

    pix.load(FILE_ICON);
    ui->fileBtn->setIcon(QPixmap(pix));

    pix.load(SCREEN_SHOT);
    ui->screenShotBtn->setIcon(QPixmap(pix));

    pix.load(CHAT_RECORD);
    ui->chatRecordBtn->setIcon(QPixmap(pix));


    //************************拉群****************************
    connect(ui->addGroupBtn,&QPushButton::clicked,this,&MainWindow::addGroup);

    //************************发信息事件***********************
    connect(ui->sendBtn,&QPushButton::clicked,this,&MainWindow::sendMessage);

    //*************************加好友或聊天操作*********************
    ui->user_info_widget->hide();  //用户信息面板
    connect(ui->doSomethingToUserBtn,&QPushButton::clicked,[=](){
        if(ui->doSomethingToUserBtn->text()=="加好友"){
            if(QMessageBox::Yes == QMessageBox::information(this,"提示","确定要加此用户为好友吗？"))
                addFriendRequest();
        }
        else
            sendMessageRequest();
    });

    //*****************点击头像设置个人信息页面*********************
    s = new SetInfomation;
    s->setPic(headIcon);
    connect(s, &SetInfomation::newInfo, this, &MainWindow::changeInfo);
    connect(s, &SetInfomation::newPic, this, &MainWindow::changePic);
    connect(ui->infoBtn,&QPushButton::clicked,[=](){
        s->setInfo(nickname, sex, signature, id);
        s->show();
    });

    //****************当前会话栏点击************************
    connect(ui->myChatBtn,&QPushButton::clicked,[=](){
        gotoPage(0);
    });

    //********************好友栏点击******************
    connect(ui->myFriendBtn,&QPushButton::clicked,[=](){
        gotoPage(1);
        restoreSearchFriend();
    });

    //加好友搜索栏回车事件
    connect(ui->searchEdit_2,&SearchWidget::returnPressed, [=](){
        if(ui->addFriendBtn->text()=="⏎")
            queryUser();
        else
            searchFriend();
    });

    //好友搜索栏点击事件
    connect(ui->searchEdit_2,&SearchWidget::changePage,[=](){
        if(ui->addFriendBtn->text()=="⏎")
            ui->search_friend_widget->setCurrentIndex(2);
        else
            ui->search_friend_widget->setCurrentIndex(1);
    });

    //加好友按钮点击切换列表
    connect(ui->addFriendBtn,&QPushButton::clicked,[=](){
        if(ui->addFriendBtn->text()=="+"){
            ui->search_friend_widget->setCurrentIndex(2);
            ui->addFriendBtn->setText("⏎");
        }
        else if(ui->addFriendBtn->text()=="⏎"){
            ui->search_friend_widget->setCurrentIndex(0);
            ui->addFriendBtn->setText("+");
        }
    });

    //****************加好友*************************
    queried_user = new QueryUserWidget;  //加好友条目
    queried_user->setParent(ui->add_friend_page);
    connect(queried_user,&QueryUserWidget::userInfo,this,&MainWindow::showUserInfoPanel);
    noThatUser = new QLabel("   查询不到此用户");
    noThatUser->setFixedHeight(25);
    noThatUser->setParent(ui->add_friend_page);

    //*****************用户信息面板其他功能按钮*************
    //更改备注按钮
    QMenu *otherFunMenu = new QMenu(this);
    QAction *setFriendRemark = new QAction("设置备注",this);
    otherFunMenu->addAction(setFriendRemark);
    ui->otherFuncBtn->setMenu(otherFunMenu);
    connect(otherFunMenu, &QMenu::triggered,this, &MainWindow::setRemark);
    //创建更改备注页面
    rw = new SetRemarkWidget;
    connect(rw, &SetRemarkWidget::newRemark,this,&MainWindow::changeRemarkRequest);

}

void MainWindow::gotoPage(int index)
{
    QPixmap pix;
    if(index==1){
        pix.load(MYFRIEND_1); //好友选中
        ui->myFriendBtn->setIcon(QPixmap(pix));
        pix.load(MYCHAT_0);  //会话非选中
        ui->myChatBtn->setIcon(QPixmap(pix));
    }
    else if(index==0){
        pix.load(MYCHAT_1);  //会话选中
        ui->myChatBtn->setIcon(QPixmap(pix));
        pix.load(MYFRIEND_0);  //好友非选中
        ui->myFriendBtn->setIcon(QPixmap(pix));
    }

    //失焦改变控件状态
    ui->search_friend_widget->setCurrentIndex(0);
    ui->addFriendBtn->setText("+");
    queried_user->hide();  //隐藏陌生人条目
    noThatUser->hide();

    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::getAnswerFromServer()  //首先与服务器建立连接，先建立登录连接，再建立聊天连接
{
    QString str = infoSocket->readAll();
    qDebug()<<str;
    if (str[0]=='1'){  //登录成功后，建立新连接
        emit loginResult(true);
        this->show();
        getMyInfo(str);
        initScene();
        queryMyFriend(); //初始化我的好友
        connect(this,&MainWindow::getFriendFinished,this,&MainWindow::queryMyGroup); //初始化我的群聊
        connect(this,&MainWindow::getGroupFinished,this,&MainWindow::showConversation);
        return;
    }
    else if(str[0]=='0'){
        emit loginResult(false);return;
    }
    if(str=="r_s"){
        emit registerResult(true);return;
    }
    else if(str=="r_f"){
        emit registerResult(false);return;
    }
    if(str[0]=='2'){  //好友信息
        getFriend(str);
        return;
    }
    else if(str[0]=='3'){  //用户信息
        getUser(str);
        return;
    }
    else if(str[0]=='7'){  //好友申请信息
        friendRequest(str);
        return;
    }
    else if(str[0]=='8'){  //好友申请是否成功
        QString info = str.right(str.length()-2);
        if(str[1]=='1')//在会话区和好友区插入一条
            friendRequestAccept(info);
        else
            QMessageBox::information(this,"提示",info+"拒绝了您的好友申请");
        return;
    }
    else if(str[0]=='9'){  //收到信息
        recvMessage(str);
        return;
    }
    else if(str[0]=='A'){  //收到拉群结果
        if(str[2]=='F')//拉群失败
            QMessageBox::information(this,"提示","群组建立失败");
        else if(str[2]=='S')
            AddGroupAccept(str);
        return;
    }
    else if(str[0]=='G'){  //收到信息
        getGroup(str);
        return;
    }
}

void MainWindow::onLogin(QString acc, QString pwd)  //发送登录账号信息到服务器
{
    QString str = "1,"+acc+","+pwd;  //1代表登录
    id = acc;
    infoSocket->write(str.toUtf8());
}

void MainWindow::onRegister(QString acc, QString pwd) //发送注册账号信息到服务器
{
    QString str = "0"+acc+","+pwd;  //0代表注册
    infoSocket->write(str.toUtf8());
}

void MainWindow::queryMyFriend()
{
    QString str = "2";
    infoSocket->write(str.toUtf8());  //2代表查询好友请求
}

void MainWindow::queryUser()
{
    QString str = "3,"+ui->searchEdit_2->text()+","+id;     //3代表查询用户
    //如果是朋友
    QMap<QString,FriendWidget*>::Iterator it = friendList.find(ui->searchEdit_2->text());
    if(it!=friendList.end()){
        queried_user->setRelation(true);
        queried_user->setInfo(it.value()->getId(),it.value()->getRemark(),it.value()->getNickname(),it.value()->getSex(),it.value()->getSignature(),it.value()->getPic());
        queried_user->show();
    }
    else
        infoSocket->write(str.toUtf8());
}

void MainWindow::changeInfo(QString name, QString sex, QString sig)
{
    nickname = name;
    this->sex = sex;
    signature = sig;
    QString str = "4,"+id+","+name+","+sex+","+sig;     //4代表更改个人信息
    infoSocket->write(str.toUtf8().data());
}

void MainWindow::changePic(QString picStr, QPixmap pic)
{
    //改变本地头像显示
    pic = pic.scaled(40,40);
    ui->infoBtn->setIcon(QPixmap(pic));

    //改变服务器储存头像显示
    QString str = "5,"+id+","+picStr;     //5代表更改个人头像
    infoSocket->write(str.toUtf8());
}

void MainWindow::changeRemarkRequest(QString newRemark)
{

//    if(queried_user->userId == ui->user_account_label->text()){
//        queried_user->setNewRemark(newRemark);
//    }

    //发送修改备注请求
    QString target = ui->user_account_label->text();
    QString str = "6,"+id+","+target+","+newRemark;     //6代表更改备注
    infoSocket->write(str.toUtf8());

    //改变本地显示
    ui->user_remark_label->setText(newRemark);
    ui->user_remark_label2->setText(newRemark);
    preFriendWidget->setRemark(newRemark);
}

void MainWindow::addFriendRequest()
{
    QString str = "7"+ui->user_account_label->text();     //7代表加好友
    infoSocket->write(str.toUtf8());
}

void MainWindow::sendMessageRequest()
{
    QString id = ui->user_account_label->text();
    QString remark = ui->user_remark_label->text();
    QString type = ui->user_tag_label->text();
    int type_int;
    if(type=="群组")type_int=1;
    else type_int = 0;
    QTime current_time = QTime::currentTime();
    int hour = current_time.hour();
    int minute = current_time.minute();
    QString time = QString::number(hour)+":"+QString::number(minute);
    QPixmap pic = ui->user_headpic_label->pixmap();

    QMap<QString,Conversation*>::Iterator it = conversationList.find(id);
    if(it==conversationList.end()){
        addConversationWidget(id,remark,pic,time,type_int,"",true);
        writeConversationToDB(id,type_int);
    }

    else
        ui->vLayout_conversation->insertWidget(0,it.value());

    changeChatInfo(id,remark,pic,type_int);
    gotoPage(0);
}

void MainWindow::sendMessage()
{
    QString message = ui->inputEdit->toPlainText();
    ui->inputEdit->clear();
    if(message.length()==0)return;
    QString target = currFriendId;
    QString messageId = QUuid::createUuid().toString();

    QMap<QString,MyChatWidget*>::Iterator it = chatRecordList.find(currFriendId);
    addRecord(it.value(),messageId,id,message,1,type);

    QSqlQuery query;
    if(type==0){
        QString str = QString("insert into friendmessage(messageId,senderId,receiverId,content,time) values('%1','%2','%3','%4',datetime('now'))").arg(messageId,id,target,message);
        if(!query.exec(str)){
            qDebug()<<"消息插入出错"<<query.lastError();
        }
        message = "90,"+target+","+message;
    }
    else{
        QString str = QString("insert into groupmessage(messageId,groupId,senderId,content,time) values('%1','%2','%3','%4',datetime('now'))").arg(messageId,target,id,message);
        if(!query.exec(str)){
            qDebug()<<"消息插入出错"<<query.lastError();
        }
        message = "91,"+target+","+message;
    }

    infoSocket->write(message.toUtf8());
}

void MainWindow::queryMyGroup()
{
    infoSocket->write("RG");
}

void MainWindow::changeChatInfo(QString id, QString remark, QPixmap pic, int t)
{
    friendPic = pic;
    currFriendId = id;
    ui->current_user_label->setText(remark);
    type = t;
    ui->inputEdit->clear();

    QMap<QString,MyChatWidget*>::Iterator it = chatRecordList.find(currFriendId);
    if(it==chatRecordList.end()){
        qDebug()<<"聊天窗口显示失败";
    }
    else{
        if(preRecordWidget&&preRecordWidget!=it.value()){
            preRecordWidget->hide();
        }
        it.value()->show();
        preRecordWidget = it.value();
    }
}

void MainWindow::updateConversationChoosedState() //更改聊天控件的选中状态
{
    Conversation *temp = qobject_cast<Conversation*>(sender());
    if(preConversationWidget){
        preConversationWidget->setIsChoosed(false);
        preConversationWidget = NULL;
    }
    preConversationWidget = temp;
}

void MainWindow::updateFriendChoosedState(int type)
{
    if(type==0)
    {
        FriendWidget *temp = qobject_cast<FriendWidget*>(sender());
        if(preFriendWidget){
            preFriendWidget->setIsChoosed(false);
            preFriendWidget = NULL;
        }
        preFriendWidget = temp;
        if(preGroupWidget){
            preGroupWidget->setIsChoosed(false);
            preGroupWidget = NULL;
        }
    }
    else
    {
        GroupWidget *temp = qobject_cast<GroupWidget*>(sender());
        if(preGroupWidget){
            preGroupWidget->setIsChoosed(false);
            preGroupWidget = NULL;
        }
        preGroupWidget = temp;
        if(preFriendWidget){
            preFriendWidget->setIsChoosed(false);
            preFriendWidget = NULL;
        }


    }



}

void MainWindow::searchFriend()
{
    QString target = ui->searchEdit_2->text();
    int len = target.length();
    for(QMap<QString,FriendWidget*>::Iterator it = friendList.begin();it!=friendList.end();it++)
    {
        if(it.value()->getRemark().contains(target) || (len>=6 && len<=15 && it.value()->getId()==target)){
            searchFriendList.append(it.value());
            ui->search_friend_Layout->insertWidget(1,it.value());
        }
    }
    for(QMap<QString,GroupWidget*>::Iterator it = groupList.begin();it!=groupList.end();it++)
    {
        if(it.value()->getName().contains(target) || (len>=6 && len<=15 && it.value()->getId()==target)){
            searchGroupList.append(it.value());
            ui->search_group_Layout->insertWidget(1,it.value());
        }
    }
}

void MainWindow::restoreSearchFriend()
{
    for(QList<FriendWidget*>::Iterator it = searchFriendList.begin();it!=searchFriendList.end();it++)
    {
        ui->friend_Layout->insertWidget(1,(*it));
    }
    for(QList<GroupWidget*>::Iterator it = searchGroupList.begin();it!=searchGroupList.end();it++)
    {
        ui->group_Layout->insertWidget(1,(*it));
    }
    searchFriendList.clear();
    searchGroupList.clear();
}


//*****************************解析服务器返回的数据**********************
void MainWindow::getMyInfo(QString str)   //初始化取得个人信息
{
    QStringList temp = str.split(",");
    QString nickname = temp[1];
    QString sex = temp[2];
    QString signature = temp[3];
    QString picStr = temp[4];
    QByteArray byteText = QByteArray::fromBase64(picStr.toLocal8Bit());

    this->nickname = nickname;
    this->sex = sex;
    this->signature = signature;
    headIcon.loadFromData(byteText,"png");
    ui->infoBtn->setIcon(QPixmap(headIcon));
}

void MainWindow::getFriend(QString str)
{
    if(str.length()<2){
        emit getFriendFinished();
        return;
    }
    QString friendId;
    QString friendNickname;
    QString friendSex;
    QString friendSignature;
    QString friendRemark;
    QString picStr;
    QStringList friends = str.split(",");
    for(int i=1;i<friends.length();i++){
        QStringList friendAttribute = friends[i].split(":");
        friendId = friendAttribute[0];
        friendNickname = friendAttribute[1];
        friendSex = friendAttribute[2];
        friendSignature = friendAttribute[3];
        friendRemark = friendAttribute[4];
        picStr = friendAttribute[5];
        QByteArray byteText = QByteArray::fromBase64(picStr.toLocal8Bit());
        QPixmap pix;
        pix.loadFromData(byteText,"png");

        qDebug()<<friendId<<friendNickname<<friendSex<<friendSignature<<friendRemark;
        addFriendWidget(friendId,friendNickname,friendSex,friendSignature,friendRemark,pix);

    }
    emit getFriendFinished();
}

void MainWindow::getGroup(QString groups)
{
    if(groups.length()<2)
    {
        emit getGroupFinished();
        return;
    }
    QVector<QString> member;
    QString groupId;
    QString groupName;
    int num;
    QByteArray picByte;
    QPixmap pic;
    QStringList eachGroup = groups.split("#");
    for(int i=1;i<eachGroup.length();i++){
        QStringList eachMember = eachGroup[i].split(",");
        QStringList eachAttribute = eachMember[0].split(":");
        groupId = eachAttribute[0];
        groupName = eachAttribute[1];
        num = eachAttribute[2].toInt();
        picByte = QByteArray::fromBase64(eachAttribute[3].toLocal8Bit());
        pic.loadFromData(picByte,"png");
        for(int j=1;j<eachMember.length();j++){
            member.push_back(eachMember[j]);
        }

        addGroupWidget(groupId,groupName,num,pic,member);
        member.clear();
    }
    //显示已有会话
    emit getGroupFinished();
}

void MainWindow::getUser(QString str)
{
    if(str.length()<2)return;
    if(str[2]==','){
        noThatUser->show();
        queried_user->hide();
        return;
    }
    noThatUser->hide();

    QStringList temp = str.split(",");
    QString userId = temp[1];
    QString userNickname = temp[2];
    QString userSex = temp[3];
    QString userSig = temp[4];
    QString picStr = temp[5];
    QByteArray headPic = QByteArray::fromBase64(picStr.toLocal8Bit());
    QPixmap pic;
    pic.loadFromData(headPic,"png");

    queried_user->setRelation(false);
    queried_user->setInfo(userId, userNickname, userNickname, userSex, userSig, pic);
    queried_user->show();
    //qDebug()<<userId<<userRemark<<userNickname<<userSex<<userSig;
}

void MainWindow::friendRequest(QString request)
{
    QStringList temp = request.split(",");
    QString sender = temp[1];
    QString nickname = temp[2];
    QString sex = temp[3];
    QString signature = temp[4];
    QString picStr = temp[5];
    QByteArray picByte = QByteArray::fromBase64(picStr.toLocal8Bit());
    QPixmap pix;
    pix.loadFromData(picByte,"png");

    FriendRequestWidget *fr = new FriendRequestWidget;
    fr->setInfo(sender,nickname,sex,signature,pix);

    connect(fr,&FriendRequestWidget::requestInfo,[=](QString sender, QString name,QString sex, QString sig, QPixmap pic){
        FriendRequestWindow *fw = new FriendRequestWindow;
        fw->setInfo(sender,name,sex,pic);
        fw->show();

        connect(fw,&FriendRequestWindow::result,[=](int result){
            if(result==1){
                QString ans = "81"+sender;
                infoSocket->write(ans.toUtf8());  //8代表接受还是拒绝好友请求，1代表接受
                addFriendWidget(sender,nickname,sex,sig,nickname,pix);
                delete fr;
            }
            else{
                QString ans = "80"+sender;   //8代表接受还是拒绝好友请求，0代表拒绝
                infoSocket->write(ans.toUtf8());
                delete fr;
            }
        });
    });
    ui->addFriend_Layout->insertWidget(1,fr);
}

void MainWindow::recvMessage(QString message)
{
    QTime time = QTime::currentTime();
    int hour = time.hour();
    int minute = time.minute();
    QString currTime = QString::number(hour)+":"+QString::number(minute);
    QString messageId = QUuid::createUuid().toString();
    if(message[1]=='0'){
        QStringList temp = message.split(",");
        QString srcId = temp[1];
        QString content = temp[2];

        QSqlQuery query;
        QString str = QString("insert into friendmessage(messageId,senderId,receiverId,content,time) values('%1','%2','%3','%4',datetime('now'))").arg(messageId,srcId,id,content);
        if(!query.exec(str)){
            qDebug()<<"消息插入出错"<<query.lastError();
        }

        QMap<QString,Conversation*>::Iterator it = conversationList.find(srcId);
        if(it != conversationList.end()){
            it.value()->updateInfo(content,currTime);
            ui->vLayout_conversation->insertWidget(0,it.value());

            MyChatWidget *targetWidget = chatRecordList[it.key()];
            addRecord(targetWidget,messageId,srcId,content,0,0);  //插入新接收的消息
        }
        else{  
            QMap<QString,FriendWidget*>::Iterator it = friendList.find(srcId);
            if(it==friendList.end()){
                qDebug()<<"出错";
                return;
            }
            QString remark = it.value()->getRemark();
            QPixmap pic = it.value()->getPic();

            addConversationWidget(srcId,remark,pic,currTime,0,content);
            bool result = writeConversationToDB(srcId,0);

//            MyChatWidget *targetWidget = chatRecordList[it.key()];
//            addRecord(targetWidget,messageId,srcId,content,0,0);
        }
    }
    else{//群聊
        QStringList temp = message.split(",");
        QString groupId = temp[1];
        QString sender = temp[2];
        QString content = temp[3];

        QSqlQuery query;
        QString str = QString("insert into groupmessage(messageId,groupId,senderId,content,time) values('%1','%2','%3','%4',datetime('now'))").arg(messageId,groupId,sender,content);
        if(!query.exec(str)){
            qDebug()<<"消息插入出错"<<query.lastError();
        }

        QMap<QString,Conversation*>::Iterator it = conversationList.find(groupId);
        if(it != conversationList.end()){
            qDebug()<<"1";
            it.value()->updateInfo(content,currTime);
            ui->vLayout_conversation->insertWidget(0,it.value());

            MyChatWidget *targetWidget = chatRecordList[it.key()];
            addRecord(targetWidget,messageId,sender,content,0,1);  //插入新接收的消息
        }
        else{
            qDebug()<<"2";
            QMap<QString,GroupWidget*>::Iterator it = groupList.find(groupId);
            if(it==groupList.end()){
                qDebug()<<"出错";
                return;
            }
            QString remark = it.value()->getName();
            QPixmap pic = it.value()->getPic();

            addConversationWidget(groupId,remark,pic,currTime,1,content);
            bool result = writeConversationToDB(groupId,1);


//            MyChatWidget *targetWidget = chatRecordList[it.key()];
//            addRecord(targetWidget,messageId,sender,content,0,1);
        }
    }
}

void MainWindow::AddGroupAccept(QString str)
{
    QStringList temp = str.split(",");
    QString groupInfo = temp[1];
    QStringList temp2 = groupInfo.split(":");
    QString groupId = temp2[0];
    QString groupName = temp2[1];
    int num = temp2[2].toInt();
    QByteArray picByte = QByteArray::fromBase64(temp2[3].toLocal8Bit());
    QPixmap pic;
    pic.loadFromData(picByte,"png");

    QVector<QString>members;
    for(int i=2;i<temp.length();i++){
        members.push_back(temp[i]);
    }

    addGroupWidget(groupId,groupName,num,pic,members);
}

void MainWindow::addConversationWidget(QString id, QString remark, QPixmap pic, QString time, int type, QString content, bool showWidget)
{
    Conversation *chatWidget = new Conversation;
    chatWidget->setType(type);
    chatWidget->setInfo(id,remark,pic,time,content);
    connect(chatWidget, &Conversation::friendInfo,this, &MainWindow::changeChatInfo);
    connect(chatWidget, &Conversation::isChoosedChanged,this, &MainWindow::updateConversationChoosedState);
    conversationList.insert(id,chatWidget);
    ui->vLayout_conversation->insertWidget(0,chatWidget);

    MyChatWidget *m = new MyChatWidget;
    m->setInfo(id,pic);
    ui->screen_area->layout()->addWidget(m);
    chatRecordList.insert(id,m);

    setRecord(m,id,type);

    if(showWidget){
        if(preRecordWidget)
            preRecordWidget->hide();
        preRecordWidget = m;
        m->show();
    }
    else
        m->hide();

}

void MainWindow::addGroupWidget(QString id,QString name,int num,QPixmap pic,QVector<QString> &member)
{
    GroupWidget *group = new GroupWidget;
    group->setGroupInfo(id,name,num,pic);

    //记录相关用户
    QString userId;
    QString userNickname;
    QString userSex;
    QString userSig;
    QString userPic;
    QByteArray picByte;
    QPixmap p;

    for(int i=0;i<member.length();i++)
    {
        QStringList temp = member[i].split(":");
        userId = temp[0];
        userNickname = temp[1];
        userSex = temp[2];
        userSig = temp[3];
        userPic = temp[4];
        picByte = QByteArray::fromBase64(userPic.toLocal8Bit());
        p.loadFromData(picByte,"png");

        User *u = new User(userId,userNickname,userSex,userSig,p);
        if(!relativeUserList.contains(userId)){
            relativeUserList.insert(userId,u);
        }
        group->setMember(userId, u);
    }

    connect(group, &GroupWidget::isChoosedChanged,this, &MainWindow::updateFriendChoosedState);
    connect(group, &GroupWidget::groupInfo,[=](QString groupId,QString groupName,int num,QPixmap pic){
        ui->user_account_label->setText(groupId);
        ui->user_nickname_label->setText(groupName+"("+QString::number(num)+")");
        ui->user_remark_label->setText(groupName);
        ui->user_remark_label2->setText(groupName);
        ui->user_tag_label->setText("群组");
        QPixmap pix;
        pix = pic.scaled(60,60);
        ui->user_headpic_label->setPixmap(pix);
        ui->user_info_widget->show();
    });
    groupList.insert(id,group);
    ui->group_Layout->insertWidget(1,group);
}

void MainWindow::addFriendWidget(QString id,QString nickname,QString sex,QString sig,QString remark,QPixmap pic)
{
    FriendWidget *goodFriend = new FriendWidget;
    goodFriend->setType(0);
    goodFriend->setInfo(id,nickname,sex,sig,remark,pic);

    User *u = new User(id,nickname,sex,sig,pic);
    if(!relativeUserList.contains(id))
        relativeUserList.insert(id,u);

    connect(goodFriend, &FriendWidget::isChoosedChanged,this, &MainWindow::updateFriendChoosedState);
    connect(goodFriend, &FriendWidget::friendInfo,[=](QString friendId,QString friendNickname,QString friendSig,QString friendRemark,QPixmap friendPic){
        ui->user_account_label->setText(friendId);
        ui->user_nickname_label->setText(friendNickname);
        ui->user_remark_label->setText(friendRemark);
        ui->user_remark_label2->setText(friendRemark);
        ui->user_signature_label->setText(friendSig);
        ui->user_tag_label->setText("用户");
        QPixmap pix;
        pix = friendPic.scaled(60,60);
        ui->user_headpic_label->setPixmap(pix);
        ui->user_info_widget->show();
    });
    friendList.insert(id,goodFriend);
    ui->friend_Layout->insertWidget(1,goodFriend);
}

void MainWindow::friendRequestAccept(QString info)
{
    QStringList temp = info.split(",");
    QString targetId = temp[0];
    QString targetNickname = temp[1];
    QString targetSex = temp[2];
    QString targetSig = temp[3];
    QString picStr = temp[4];
    QByteArray pic = QByteArray::fromBase64(picStr.toLocal8Bit());
    QPixmap pix;
    pix.loadFromData(pic,"png");
    if(ui->user_account_label->text()==targetId){
        ui->doSomethingToUserBtn->setText("发消息");
    }
    QMessageBox::information(this,"提示",targetId+targetNickname+"接受了您的好友申请");
    addFriendWidget(targetId,targetNickname,targetSex,targetSig,targetNickname,pix);
}

void MainWindow::showUserInfoPanel(QString id, QString remark, QString name, QString sex, QString sig, QPixmap pic)
{
    ui->user_info_widget->show();
    if(queried_user->getRelation()){
        ui->doSomethingToUserBtn->setText("发消息");
        ui->user_remark_label->setText(remark);
        ui->user_remark_label2->setText(remark);
    }
    else{
        ui->doSomethingToUserBtn->setText("加好友");
        ui->user_remark_label->setText(name);
        ui->user_remark_label2->setText(name);
    }
    ui->user_nickname_label->setText(name);
    ui->user_account_label->setText(id);
    if(sex == "男")
        ui->user_sex_label->setText("♂");
    else
        ui->user_sex_label->setText("♀");
    ui->user_signature_label->setText(sig);

    pic = pic.scaled(60,60);
    ui->user_headpic_label->setPixmap(pic);
}

void MainWindow::setRemark()
{
    rw->show();
    rw->setOriginalRemark(ui->user_remark_label->text());
}

void MainWindow::setRecord(MyChatWidget *w, QString target, int type)
{
    QSqlQuery query;
    QString str;
    if(type==0)
        str = QString("select messageId,senderId,receiverId,content,time from friendmessage where (senderId = '%1' and receiverId = '%2') or (senderId = '%3' and receiverId = '%4')").arg(id).arg(target).arg(target).arg(id);
    else
        str = QString("select messageId,groupId,senderId,content,time from groupmessage where groupId = '%1'").arg(target);

    if(!query.exec(str)){
        qDebug()<<"聊天记录放置失败"<<query.lastError();
        return;
    }

    QLayout *vl = w->findChild<QWidget*>("scrollAreaWidgetContents")->layout();
    if(type==0){
        while(query.next()){
            if(query.value(1).toString()==id){
                MyMessage *mm = new MyMessage;
                //mm->setParent(w);
                vl->addWidget(mm);
                mm->setInfo(query.value(0).toString(), query.value(3).toString(), headIcon);
            }
            else{
                FriendMessage *fm = new FriendMessage;
                //fm->setParent(w);
                vl->addWidget(fm);
                fm->setInfo(query.value(0).toString(), query.value(3).toString(), w->getPic());
            }
        }
    }
    else{
        while(query.next()){
            if(query.value(2).toString()==id){
                MyMessage *mm = new MyMessage;
                vl->addWidget(mm);
                mm->setInfo(query.value(0).toString(), query.value(3).toString(), headIcon);
            }
            else{
                FriendMessage *fm = new FriendMessage;
                vl->addWidget(fm);
                QString sender = query.value(2).toString();
                if(relativeUserList.contains(sender)){
                    fm->setInfo(query.value(0).toString(), query.value(3).toString(), relativeUserList[sender]->getHeadPic());
                }
                else
                    qDebug()<<"群里没有此用户";
            }
        }
    }
}

void MainWindow::addRecord(MyChatWidget *w, QString messageId, QString sender, QString content, int sor, int type)
{
    QLayout *vl = w->findChild<QWidget*>("scrollAreaWidgetContents")->layout();
    if(sor==0)//我收到
    {
        FriendMessage *fm = new FriendMessage;
        vl->addWidget(fm);
        if(type==1){
            fm->setInfo(messageId,content,relativeUserList[sender]->getHeadPic());
        }
        else
            fm->setInfo(messageId,content,w->getPic());
    }
    else//我发送
    {
        MyMessage *mm = new MyMessage;
        vl->addWidget(mm);
        mm->setInfo(messageId,content,headIcon);
    }
}

bool MainWindow::writeConversationToDB(QString id, int type)
{
    QSqlQuery query;
    QString str = QString("insert into conversation(targetId,myId,type) values('%1','%2','%3')").arg(id).arg(this->id).arg(type);
    if(!query.exec(str)){
        qDebug()<<"会话插入失败"<<query.lastError();
        return false;
    }
    return true;
}

void MainWindow::showConversation()
{
    QSqlQuery query;
    QSqlQuery query2;
    QString str = QString("select * from conversation where myId = '%1'").arg(id);
    if(!query.exec(str)){
        qDebug()<<"当前账号会话查询失败"<<query.lastError();
        return;
    }
    QString str2;
    while(query.next()){
        if(query.value(2).toInt()==0){
            QMap<QString,FriendWidget*>::Iterator it = friendList.find(query.value(0).toString());
            if(it==friendList.end()){
                qDebug()<<"会话查找出错";
            }
            else{
                QString targetId = query.value(0).toString();
                QString remark = it.value()->getRemark();
                QPixmap pic = it.value()->getPic();
                str2 = QString("select content,time from friendmessage where (senderId='%1' and receiverId = '%2') or (senderId='%3' and receiverId = '%4') order by sequence desc limit 1").arg(id).arg(targetId).arg(targetId).arg(id);
                if(!query2.exec(str2)){
                    qDebug()<<"查询最后一条对话失败"<<query2.lastError();
                }
                QString content;
                QString time;
                if(query2.next()){
                    content = query2.value(0).toString();
                    QDateTime time1 = query2.value(1).toDateTime();
                    QDateTime time2 = QDateTime::currentDateTime();
                    QString timeStr = time1.toString("yyyy-MM-dd hh:mm::ss.zzz");
                    if(time1.daysTo(time2)>0)
                        time = timeStr.mid(5,5);
                    else if(time1.daysTo(time2)==0)
                        time = timeStr.mid(11,5);
                }
                addConversationWidget(targetId,remark,pic,time,0,content);
            }
        }
        else{
            QMap<QString,GroupWidget*>::Iterator it = groupList.find(query.value(0).toString());
            if(it==groupList.end()){
                qDebug()<<"会话查找出错";
            }
            else{
                QString groupId = query.value(0).toString();
                QString groupName = it.value()->getName();
                QPixmap pic = it.value()->getPic();
                str2 = QString("select content,time from groupmessage where groupId='%1' order by sequence desc limit 1").arg(groupId);
                if(!query2.exec(str2)){
                    qDebug()<<"查询最后一条对话失败"<<query2.lastError();
                }
                QString content;
                QString time;
                if(query2.next()){
                    content = query2.value(0).toString();
                    QDateTime time1 = query2.value(1).toDateTime();
                    QDateTime time2 = QDateTime::currentDateTime();
                    QString timeStr = time1.toString("yyyy-MM-dd hh:mm::ss.zzz");
                    if(time1.daysTo(time2)>0)
                        time = timeStr.mid(5,5);
                    else if(time1.daysTo(time2)==0)
                        time = timeStr.mid(11,5);
                }
                addConversationWidget(groupId,groupName,pic,time,1,content);
            }
        }
    }
}

void MainWindow::addGroup()
{
    AddGroup *a = new AddGroup(friendList);
    a->setParent(this,Qt::Window|Qt::FramelessWindowHint);
    connect(a,&AddGroup::members,[=](QString member){
        QString groupMembers = "G"+member; //向服务器发送群组请求
        infoSocket->write(groupMembers.toUtf8());
    });
    a->show();

}






