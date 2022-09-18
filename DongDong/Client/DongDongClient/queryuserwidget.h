#ifndef QUERYUSERWIDGET_H
#define QUERYUSERWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class QueryUserWidget;
}

class QueryUserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryUserWidget(QWidget *parent = nullptr);
    ~QueryUserWidget();

    void mouseReleaseEvent(QMouseEvent*);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);

    bool getIsChoosed();
    void setIsChoosed(bool);

    void setInfo(QString,QString,QString,QString,QString,QPixmap);
    void setNewRemark(QString);
    void setRelation(bool);
    bool getRelation();


private:
    Ui::QueryUserWidget *ui;

    QString userId;
    QString remark;
    QString nickname;
    QPixmap headPic;
    QString signature;
    QString userSex;
    bool relation;

signals:
    void userInfo(QString,QString,QString,QString,QString,QPixmap);
};

#endif // QUERYUSERWIDGET_H
