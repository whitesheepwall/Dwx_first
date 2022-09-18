#ifndef CHOOSEDFRIEND_H
#define CHOOSEDFRIEND_H

#include <QWidget>
#include "cancelsign.h"

namespace Ui {
class ChoosedFriend;
}

class ChoosedFriend : public QWidget
{
    Q_OBJECT

public:
    explicit ChoosedFriend(QWidget *parent = nullptr);
    ~ChoosedFriend();

    void setInfo(QString,QString,QPixmap);

    QString id;
    QString remark;
    QPixmap pic;//头像
    QPixmap pix;//选中标识

private:
    Ui::ChoosedFriend *ui;

signals:
    void cancelChoose(QString);
};

#endif // CHOOSEDFRIEND_H
