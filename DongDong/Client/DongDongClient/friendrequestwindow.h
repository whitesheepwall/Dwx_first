#ifndef FRIENDREQUESTWINDOW_H
#define FRIENDREQUESTWINDOW_H

#include <QWidget>

namespace Ui {
class FriendRequestWindow;
}

class FriendRequestWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequestWindow(QWidget *parent = nullptr);
    ~FriendRequestWindow();

    void setInfo(QString,QString,QString,QPixmap);

private:
    Ui::FriendRequestWindow *ui;

signals:
    void result(int);
};

#endif // FRIENDREQUESTWINDOW_H
