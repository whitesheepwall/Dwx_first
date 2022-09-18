#ifndef FRIENDREQUESTWIDGET_H
#define FRIENDREQUESTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class FriendRequestWidget;
}

class FriendRequestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequestWidget(QWidget *parent = nullptr);
    ~FriendRequestWidget();

    void mouseReleaseEvent(QMouseEvent*);
    void setInfo(QString,QString,QString,QString,QPixmap);
    void enterEvent(QEnterEvent*);
    void leaveEvent(QEvent*);


private:
    Ui::FriendRequestWidget *ui;
    QString id;
    QString nickname;
    QString sex;
    QString signature;
    QPixmap pic;

signals:
    void requestInfo(QString,QString,QString,QString,QPixmap);

};

#endif // FRIENDREQUESTWIDGET_H
