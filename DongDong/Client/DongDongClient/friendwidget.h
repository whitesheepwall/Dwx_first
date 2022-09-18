#ifndef FRIENDWIDGET_H
#define FRIENDWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "config.h"
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class FriendWidget;
}

class FriendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendWidget(QWidget *parent = nullptr);
    ~FriendWidget();

    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);
    bool getIsChoosed();
    void setIsChoosed(bool);
    void setInfo(QString, QString, QString, QString, QString, QPixmap);
    void setType(int);
    void setRemark(QString);
    QString getId();
    QString getRemark();
    QString getNickname();
    QString getSex();
    QString getSignature();
    QPixmap getPic();

private:
    Ui::FriendWidget *ui;
    QString friendId;
    QString nickname;
    QString sex;
    QString signature;
    QString remark;
    QPixmap headPic;
    bool isChoosed;
    int type;

signals:
    void friendInfo(QString,QString,QString,QString,QPixmap);
    void isChoosedChanged(int);
};

#endif // FRIENDWIDGET_H
