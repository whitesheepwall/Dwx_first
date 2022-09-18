#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QHash>
#include <QMouseEvent>
#include "user.h"

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GroupWidget(QWidget *parent = nullptr);
    ~GroupWidget();

    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);
    bool getIsChoosed();
    void setIsChoosed(bool);
    void setGroupInfo(QString,QString,int,QPixmap);
    void setMember(QString id, User*);
    QString getId();
    QString getName();
    QPixmap getPic();

private:
    Ui::GroupWidget *ui;
    QString groupId;
    QString groupName;
    QPixmap groupPic;
    QHash<QString, User*> groupMember;
    bool isChoosed;
    int num;

signals:
    void groupInfo(QString,QString,int,QPixmap);
    void isChoosedChanged(int);

};

#endif // GROUPWIDGET_H
