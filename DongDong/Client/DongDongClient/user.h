#ifndef USER_H
#define USER_H

#include <QPixmap>
#include <QString>

class User
{
public:
    User(QString,QString,QString,QString,QPixmap);

    void setId(QString);
    void setNickName(QString);
    void setSex(QString);
    void setSignature(QString);
    void setHeadPic(QPixmap);

    QString getId();
    QString getNickName();
    QString getSex();
    QString getSignature();
    QPixmap getHeadPic();

private:
    QString id;
    QString nickname;
    QString sex;
    QString signature;
    QPixmap headPic;
};

#endif // USER_H
