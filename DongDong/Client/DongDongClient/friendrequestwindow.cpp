#include "friendrequestwindow.h"
#include "ui_friendrequestwindow.h"

FriendRequestWindow::FriendRequestWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendRequestWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->agreeBtn,&QPushButton::clicked,[=](){
        emit result(1);
        this->close();
    });
    connect(ui->refuseBtn,&QPushButton::clicked,[=](){
        emit result(0);
        this->close();
    });
}

FriendRequestWindow::~FriendRequestWindow()
{
    delete ui;
}

void FriendRequestWindow::setInfo(QString id, QString name, QString sex, QPixmap pic)
{
    ui->nickname_label->setText(name);
    ui->account_label->setText(id);
    ui->sex_label->setText(sex);

    pic = pic.scaled(50,50);
    ui->headIcon_label->setPixmap(pic);
}
