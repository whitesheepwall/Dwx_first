#include "friendrequestwidget.h"
#include "ui_friendrequestwidget.h"

FriendRequestWidget::FriendRequestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendRequestWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);  //顶层窗口默认不生效，让顶层窗口样式生效
    this->setAutoFillBackground(true);
}

FriendRequestWidget::~FriendRequestWidget()
{
    delete ui;
}

void FriendRequestWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        emit requestInfo(id,nickname,sex,signature,pic);
    }
}

void FriendRequestWidget::setInfo(QString id, QString nickname, QString sex, QString sig, QPixmap pic)
{
    this->id = id;
    this->nickname = nickname;
    this->sex = sex;
    this->signature = sig;
    this->pic = pic;
    ui->name_label->setText(nickname+"("+id+")");
    if(sex=="男")
        ui->sex_label->setText("♂");
    else
        ui->sex_label->setText("♀");

    pic = pic.scaled(40,40);
    ui->head_icon_label->setPixmap(pic);
}

void FriendRequestWidget::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void FriendRequestWidget::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(232,232,232));
        this->setPalette(palette);
    }
}
