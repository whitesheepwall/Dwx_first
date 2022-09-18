#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QWidget>
#include "friendwidget.h"
#include "choosefriend.h"
#include "choosedfriend.h"

namespace Ui {
class AddGroup;
}

class AddGroup : public QWidget
{
    Q_OBJECT

public:
    explicit AddGroup(const QMap<QString,FriendWidget*>&);
    ~AddGroup();

    void choosed(QString,QString,QPixmap);
    void cancelChoosed(QString);
    void returnResult();

private:
    Ui::AddGroup *ui;
    int sumChoosed;
    QMap<QString,ChooseFriend*> chooseFriendList;
    QMap<QString,ChoosedFriend*> choosedFriendList;

signals:
    void members(QString);
};

#endif // ADDGROUP_H
