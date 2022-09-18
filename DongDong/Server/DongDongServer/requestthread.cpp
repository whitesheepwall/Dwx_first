#include "requestthread.h"
#include "globalvariable.h"

RequestThread::RequestThread(QTcpSocket *t, QString req, QString id)
{
    temp = t;
    str = req;
    srcId = id;
    setAutoDelete(true);
    res = "";

    //获得数据库连接
    db = SqlConnectionPool::openConnection();

}

void RequestThread::run()
{
    if(!db.open()){
        qDebug()<<"子线程数据库打开失败";
        return;
    }
    qDebug()<<temp<<str<<srcId;
    if(str[0]=='1'){  //登录请求
        QString loginResult = readLoginAccount(str);
        //如果第一位是1，说明登录成功，记录登录用户的id-socket对到链表里
        if(loginResult[0]=='1'){
            QString account = str.split(",")[1];
            temp->setObjectName(account);

            GlobalVariable::clientMutex.lock();
            GlobalVariable::clientRequestMap.insert(account,temp);
            GlobalVariable::clientMutex.unlock();

            srcId = account;
            emit onlinePlus();

            //初始化群组成员信息
            updateGroupMember(account,temp);

        }
        //temp->write(loginResult.toUtf8());
        res = loginResult;
        //emit response(loginResult);
    }
    else if(str[0]=='0'){  //注册请求
        QString registerResult = readRegisterAccount(str);
        //temp->write(registerResult.toUtf8());
        res = registerResult;
    }
    else if(str[0]=='2'){   //获取好友列表请求
        QString friendInfo = readFriend(srcId);
        //temp->write(friendInfo.toUtf8());
        res = friendInfo;
    }
    else if(str[0]=='3'){   //查询用户请求
        QString userInfo = queryUser(str);
        //temp->write(userInfo.toUtf8());
        res = userInfo;
    }
    else if(str[0]=='4')updateUserInfo(str);//修改信息请求
    else if(str[0]=='5')updateUserPic(str);//修改头像请求
    else if(str[0]=='6')updateUserRemark(str);//修改备注请求
    else if(str[0]=='7')addFriendRequest(srcId, str);//加好友请求
    else if(str[0]=='8'){   //加好友结果
        QString result = addFriendResult(srcId, str);
        QString sender = str.right(str.length()-2);

        GlobalVariable::clientMutex.lock();
        QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(sender);
        if(it!=GlobalVariable::clientRequestMap.end()){
            res = result;
            //it.value()->write(result.toUtf8());
        }
        GlobalVariable::clientMutex.unlock();
        //目标用户不在线，记录在延迟发送表里
    }
    else if(str[0]=='G'){   //拉群请求
        bool result = addGroup(srcId, str);
        if(!result){
            //temp->write("AGF");
            res = "AGF";
        }
    }
    else if(str=="RG"){
        QString groups = readGroup(srcId);
        //temp->write(groups.toUtf8());
        res = groups;
    }

    db.close();
    SqlConnectionPool::closeConnection(db);

    QMap<QString,QString> map;
    map.insert(srcId,res);
qDebug()<<srcId<<res;
qDebug()<<"*************************";
    GlobalVariable::msgQMutex.lock();
    GlobalVariable::msgQ.emplace_back(map);
    GlobalVariable::msgQMutex.unlock();
    emit response();

    return;
}

void RequestThread::updateGroupMember(QString account, QTcpSocket* temp)
{
    QString str = QString("select groupId from userGroup where userId='%1'").arg(account);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"更新群组信息失败";
    }

    GlobalVariable::groupMutex.lock();
    while(query.next()){
        if(GlobalVariable::groupsMembersList.contains(query.value(0).toString())){
            QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::groupsMembersList[query.value(0).toString()].find(account);
            if(it!=GlobalVariable::groupsMembersList[query.value(0).toString()].end()){
                it.value() = temp;
            }
        }
    }
    GlobalVariable::groupMutex.unlock();
}

QString RequestThread::readLoginAccount(QString str)
{
    QStringList temp = str.split(",");
    QString account = temp[1];
    QString password = temp[2];
    str = QString("select * from account where userId='%1' and password='%2'").arg(account).arg(password);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"登录查询失败";
        return "l_f";
    }
    if(query.next()){
        //登录成功，发送基本信息给用户
        emit showLog(account+"登录成功");
        QString nickname = query.value(1).toString();
        QString sex = query.value(2).toString();
        QString signature = query.value(3).toString();
        //读出图片byteArray信息
        QByteArray byteText = query.value(4).toByteArray();
        //转成Base64字符串
        QString picStr = QString(byteText.toBase64());
        return "1,"+nickname+","+sex+","+signature+","+picStr; //login_success
    }
    else{
        emit showLog(account+"登录失败");
        return "l_f"; //login_failed
    }
}

QString RequestThread::readRegisterAccount(QString str)
{
    QString account;
    QString password;


    int i=1;
    int temp = i;
    while(i<str.length()){  //分离账号密码
        if(str[i]==',')break;
        ++i;
    }
    account = str.mid(temp,i-temp);
    ++i;
    password = str.right(str.length()-i);

    str = QString("select * from account where userId='%1'").arg(account);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"注册查询失败";
        return "r_f";
    }
    if(query.next()){  //查到账号说明账号重复
        return "r_f"; //register_failed
    }
    else{
        str = QString("insert into account(userId,password,nickname,sex,signature,headIcon) values(:id,:pwd,:nick,:sex,:sig,:icon)");
        query.prepare(str);
        query.bindValue(":id",account);
        query.bindValue(":pwd",password);
        query.bindValue(":nick",account);
        query.bindValue(":sex","男");
        query.bindValue(":sig","此人还没有个性签名哦");
        query.bindValue(":icon",GlobalVariable::defaultHeadIcon);
        if(!query.exec()){
            qDebug()<<"插入账号失败";
            return "r_f";
        }
        emit showLog(account+"注册成功");
        return "r_s";  //register_success
    }
}

QString RequestThread::readFriend(QString account)
{
    QSqlQuery query2(NULL,db);
    QString str = QString("select friendId,nickname,sex,signature,remark from friend where userId='%1'").arg(account);
    QSqlQuery query(str,db);
    QString str2;
    QString friendId;
    QString friendNickname;
    QString friendSex;
    QString sig;
    QString remark;
    QString picStr;
    QString ans = "2";
    if(!query.exec()){
        qDebug()<<"好友查询失败";
        return ans;
    }
    while(query.next())
    {
        friendId = query.value(0).toString();
        friendNickname = query.value(1).toString();
        friendSex = query.value(2).toString();
        sig = query.value(3).toString();
        remark = query.value(4).toString();
        str2 = QString("select headIcon from account where userId='%1'").arg(friendId);

        if(!query2.exec(str2)){
            qDebug()<<"好友头像查询失败";
        }
        if(query2.next()){
            picStr = query2.value(0).toByteArray().toBase64();
        }
        ans+=',';
        ans+=friendId+":"+friendNickname+":"+friendSex+":"+sig+":"+remark+":"+picStr;
    }
    return ans;
}

QString RequestThread::queryUser(QString request)
{
    QStringList temp = request.split(',');
    QString target = temp[1];
    QString src = temp[2];

    QString ans = "3";  
    QString str = QString("select userId,nickname,sex,signature,headIcon from account where userId='%1'").arg(target);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"用户查询失败";
        return ans;
    }

    QString id;
    QString nickname;
    QString sex;
    QString sig;
    QString picStr;
    if(query.next()){
        id = query.value(0).toString();
        nickname = query.value(1).toString();
        sex = query.value(2).toString();
        sig = query.value(3).toString();
        picStr = query.value(4).toByteArray().toBase64();
    }
    ans+=","+id+","+nickname+","+sex+","+sig+","+picStr;
    return ans;
}

void RequestThread::updateUserInfo(QString info)
{
    QStringList temp = info.split(",");
    QString id = temp[1];
    QString nickname = temp[2];
    QString sex = temp[3];
    QString signature = temp[4];

    QString str = QString("update account set nickname='%1',sex='%2',signature='%3' where userId='%4'").arg(nickname).arg(sex).arg(signature).arg(id);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"更新信息失败";
    }
    str = QString("update friend set nickname='%1',sex='%2',signature='%3' where friendId='%4'").arg(nickname).arg(sex).arg(signature).arg(id);
    if(!query.exec(str)){
        qDebug()<<"更新信息失败";
    }
}

void RequestThread::updateUserPic(QString content)
{
    //1、先分离id和图片序列，id是纯string，图片是Base64加密后的字符串
    QStringList temp = content.split(",");
    QString id = temp[1];

    //2、解密还原图片，转回QByteArray类型
    QString picStr = temp[2];
    QByteArray picByte = QByteArray::fromBase64(picStr.toLocal8Bit());

    //3、将QByteArray数据写入数据库
    QString str = QString("update account set headIcon=:pic where userId=:id");
    QSqlQuery query(NULL,db);
    query.prepare(str);
    query.bindValue(":pic",picByte);
    query.bindValue(":id",id);

    if(!query.exec()){
        qDebug()<<"更新头像失败"<<query.lastError();
    }
}

void RequestThread::updateUserRemark(QString str)
{
    QStringList temp = str.split(",");
    QString id = temp[1];
    QString target = temp[2];
    QString newRemark = temp[3];

    QString str2 = QString("update friend set remark='%1' where userId='%2' and friendId='%3'").arg(newRemark).arg(id).arg(target);
    QSqlQuery query(str2,db);
    if(!query.exec()){
        qDebug()<<"修改备注失败";
        return;
    }
}

void RequestThread::addFriendRequest(QString src, QString request)
{
    QString target = request.right(request.length()-1);

    QString str = QString("select nickname, sex, signature, headIcon from account where userId = '%1'").arg(src);
    QSqlQuery query(str,db);
    if(!query.exec()){
        qDebug()<<"源用户信息查询失败";
        return;
    }
    if(!query.next()){
        qDebug()<<"加好友源用户查询出错";
        return;
    }
    QString ans = "7,"+src+",";
    QString picStr = QString(query.value(3).toByteArray().toBase64());
    ans+=query.value(0).toString()+","+query.value(1).toString()+","+query.value(2).toString()+","+picStr;

    GlobalVariable::clientMutex.lock();
    QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(target);
    if(it!=GlobalVariable::clientRequestMap.end())
        it.value()->write(ans.toUtf8());
    GlobalVariable::clientMutex.unlock();
}

QString RequestThread::addFriendResult(QString id, QString sender)
{
    QString nickname;
    QString sex;
    QString sig;
    if(sender[1]=='1'){ //同意加好友
        sender = sender.right(sender.length()-2);
        QSqlQuery query(NULL,db);
        QString str = QString("select * from account where userId='%1'").arg(sender);
        if(!query.exec(str)){
            qDebug()<<"加好友查询出错";
            return "Error";
        }
        if(!query.next()){
            qDebug()<<"加好友next出错";
            return "Error";
        }
        nickname = query.value(1).toString();
        sex = query.value(2).toString();
        sig = query.value(3).toString();
        str = QString("insert into friend(friendId,userId,nickname,sex,signature,remark) values('%1','%2','%3','%4','%5','%6')").arg(sender).arg(id).arg(nickname).arg(sex).arg(sig).arg(nickname);
        if(!query.exec(str)){
            qDebug()<<"加好友插入出错";
            return "Error";
        }

        str = QString("select * from account where userId='%1'").arg(id);
        if(!query.exec(str)){
            qDebug()<<"加好友查询出错";
            return "Error";
        }
        if(!query.next()){
            qDebug()<<"加好友next出错";
            return "Error";
        }
        nickname = query.value(1).toString();
        sex = query.value(2).toString();
        sig = query.value(3).toString();
        QString picStr = query.value(4).toByteArray().toBase64();
        str = QString("insert into friend(friendId,userId,nickname,sex,signature,remark) values('%1','%2','%3','%4','%5','%6')").arg(id).arg(sender).arg(nickname).arg(sex).arg(sig).arg(nickname);
        if(!query.exec(str)){
            qDebug()<<"加好友插入出错";
            return "Error";
        }
        return "81"+id+","+nickname+","+sex+","+sig+","+picStr;
    }
    return "80"+id+"-"+nickname;
}

bool RequestThread::addGroup(QString srcId, QString member)
{
    qDebug()<<member;
    QStringList temp = member.split(",");
    int num = temp[0].right(temp[0].length()-1).toInt();
    QString groupId = QUuid::createUuid().toString();
    QString groupName = "群聊";
    QSqlQuery query(NULL,db);
    QString str = QString("insert into groups(groupId,groupName,groupIcon,numOfMember) values(:groupId,:groupName,:groupIcon,:numOfMember)");
    query.prepare(str);
    query.bindValue(":groupId",groupId);
    query.bindValue(":groupName",groupName);
    query.bindValue(":groupIcon",GlobalVariable::defaultGroupIcon);
    query.bindValue(":numOfMember",num);
    if(!query.exec()){
        qDebug()<<"群组插入失败"<<query.lastError();
        return false;
    }
    QString members;
    QSqlQuery query2(NULL,db);
    str = QString("insert into userGroup(userId,groupId) values('%1','%2')").arg(srcId).arg(groupId);
    if(!query.exec(str)){
        qDebug()<<"用户插入群失败"<<query.lastError();
        return false;
    }
    members+=srcId;
    QMap<QString,QTcpSocket*> t;
    QString str2 = QString("select nickname,sex,signature,headIcon from account where userId='%1'").arg(srcId);
    if(!query2.exec(str2)){
        qDebug()<<"加群用户查询失败"<<query.lastError();
    }
    if(query2.next()){
        QString nickname = query2.value(0).toString();
        QString sex = query2.value(1).toString();
        QString sig = query2.value(2).toString();
        QString pic = query2.value(3).toByteArray().toBase64();
        members+=":"+nickname+":"+sex+":"+sig+":"+pic;

        //计入群组用户组里
        GlobalVariable::clientMutex.lock();
        QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(srcId);
        if(it!=GlobalVariable::clientRequestMap.end()){
            t.insert(srcId,it.value());
        }
        else{
            t.insert(srcId,NULL);
        }
        GlobalVariable::clientMutex.unlock();
    }

    for(int i=1;i<num;i++){
        str = QString("insert into userGroup(userId,groupId) values('%1','%2')").arg(temp[i]).arg(groupId);
        if(!query.exec(str)){
            qDebug()<<"用户插入群失败"<<query.lastError();
            return false;
        }
        members+=","+temp[i];
        str2 = QString("select nickname,sex,signature,headIcon from account where userId='%1'").arg(temp[i]);
        if(!query2.exec(str2)){
            qDebug()<<"加群用户查询失败"<<query.lastError();
        }
        while(query2.next()){
            QString nickname = query2.value(0).toString();
            QString sex = query2.value(1).toString();
            QString sig = query2.value(2).toString();
            QString pic = query2.value(3).toByteArray().toBase64();
            members+=":"+nickname+":"+sex+":"+sig+":"+pic;

            //计入群组用户组里
            GlobalVariable::clientMutex.lock();
            QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(temp[i]);
            if(it!=GlobalVariable::clientRequestMap.end()){
                t.insert(temp[i],it.value());
            }
            else{
                t.insert(temp[i],NULL);
            }
            GlobalVariable::clientMutex.unlock();
        }
    }
    GlobalVariable::groupMutex.lock();
    GlobalVariable::groupsMembersList.insert(groupId,t);
    GlobalVariable::groupMutex.unlock();

    QString pic = QString(GlobalVariable::defaultGroupIcon.toBase64());

    GlobalVariable::clientMutex.lock();
    QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(srcId);
    if(it!=GlobalVariable::clientRequestMap.end()){
        QString ans = "AGS,"+groupId+":"+groupName+":"+QString::number(num)+":"+pic+","+members;
        it.value()->write(ans.toUtf8());
    }
    for(int i=1;i<num;i++){
        QMap<QString,QTcpSocket*>::Iterator it = GlobalVariable::clientRequestMap.find(temp[i]);
        if(it!=GlobalVariable::clientRequestMap.end()){
            QString ans = "AGS,"+groupId+":"+groupName+":"+QString::number(num)+":"+pic+","+members;
            it.value()->write(ans.toUtf8());
        }
    }
    GlobalVariable::clientMutex.unlock();
    return true;
}

QString RequestThread::readGroup(QString srcId)
{
    QVector<QString> groupIdVector;
    QSqlQuery query(NULL,db);
    QSqlQuery query2(NULL,db);
    QSqlQuery query3(NULL,db);
    QSqlQuery query4(NULL,db);
    QString str = QString("select groupId from userGroup where userId='%1'").arg(srcId);
    QString str2;
    QString str3;
    QString groupId;
    QString groupName;
    QString groupNum;
    QString groupPic;
    QString userId;
    QString userNickname;
    QString userSex;
    QString userSig;
    QString userPic;
    QString ans="G";
    if(!query.exec(str)){
        qDebug()<<"群组查询失败";
        return "";
    }
    while(query.next()){  //取得用户所有的群
        //groupIdVector.push_back(query.value(0).toString());

        str = QString("select * from groups where groupId='%1'").arg(query.value(0).toString());  //取得群的信息
        if(!query2.exec(str)){
            qDebug()<<"群组信息查询失败";
            continue;
        }
        if(query2.next()){
            groupId = query2.value(0).toString();
            groupName = query2.value(1).toString();
            groupPic = query2.value(2).toByteArray().toBase64();
            groupNum = query2.value(3).toString();
            ans+="#";
            ans+=groupId+":"+groupName+":"+groupNum+":"+groupPic;
        }
        str2 = QString("select userId from userGroup where groupId='%1'").arg(query.value(0).toString()); //取得当前群里所有的成员
        if(!query3.exec(str2)){
            qDebug()<<"群组成员查询失败";
        }
        while(query3.next()){
            str3 = QString("select nickname,sex,signature,headIcon from account where userId='%1'").arg(query3.value(0).toString());  //取得群成员信息
            if(!query4.exec(str3)){
                qDebug()<<"成员信息查询失败";
            }
            if(query4.next()){
                userId = query3.value(0).toString();
                userNickname = query4.value(0).toString();
                userSex = query4.value(1).toString();
                userSig = query4.value(2).toString();
                userPic = query4.value(3).toByteArray().toBase64();
                ans+=",";
                ans+=userId+":"+userNickname+":"+userSex+":"+userSig+":"+userPic;
            }
        }

    }
    qDebug()<<ans;
    return ans;
}
