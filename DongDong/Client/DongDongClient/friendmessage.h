#ifndef FRIENDMESSAGE_H
#define FRIENDMESSAGE_H

#include <QWidget>

namespace Ui {
class FriendMessage;
}

class FriendMessage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendMessage(QWidget *parent = nullptr);
    ~FriendMessage();

    void setInfo(QString,QString,QPixmap);

private:
    Ui::FriendMessage *ui;

    QString messageId;
};

#endif // FRIENDMESSAGE_H
