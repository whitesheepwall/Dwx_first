#include "mymessage.h"
#include "ui_mymessage.h"

MyMessage::MyMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyMessage)
{
    ui->setupUi(this);
}

MyMessage::~MyMessage()
{
    delete ui;
}

void MyMessage::setInfo(QString id, QString content,QPixmap pic)
{
    messageId = id;
    ui->text_label->setText(content);
    pic = pic.scaled(40,40);
    ui->headPic_label->setPixmap(pic);
}
