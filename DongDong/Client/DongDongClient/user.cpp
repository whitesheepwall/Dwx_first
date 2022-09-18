#include "user.h"

User::User(QString id,QString nickname,QString sex,QString signature,QPixmap headPic)
{
    this->id = id;
    this->nickname = nickname;
    this->sex = sex;
    this->signature = signature;
    this->headPic = headPic;
}

void User::setId(QString id)
{
    this->id = id;
}

void User::setNickName(QString nickname)
{
    this->nickname = nickname;
}

void User::setSex(QString sex)
{
    this->sex = sex;
}

void User::setSignature(QString signature)
{
    this->signature = signature;
}

void User::setHeadPic(QPixmap headPic)
{
    this->headPic = headPic;
}

QString User::getId()
{
    return id;
}

QString User::getNickName()
{
    return nickname;
}

QString User::getSex()
{
    return sex;
}

QString User::getSignature()
{
    return signature;
}

QPixmap User::getHeadPic()
{
    return headPic;
}
