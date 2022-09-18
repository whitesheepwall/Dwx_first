#include "friendmessage.h"
#include "ui_friendmessage.h"

FriendMessage::FriendMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendMessage)
{
    ui->setupUi(this);
}

FriendMessage::~FriendMessage()
{
    delete ui;
}

void FriendMessage::setInfo(QString id, QString content, QPixmap pic)
{
    messageId = id;
    ui->text_label->setText(content);
    pic = pic.scaled(40,40);
    ui->headPic_label->setPixmap(pic);
}
