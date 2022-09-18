#include "friendwidget.h"
#include "ui_friendwidget.h"

FriendWidget::FriendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendWidget)
{
    ui->setupUi(this);
    setFixedSize(251,70);
    setAttribute(Qt::WA_StyledBackground);  //顶层窗口默认不生效，让顶层窗口样式生效
    this->setAutoFillBackground(true);
    isChoosed = false;
}

FriendWidget::~FriendWidget()
{
    delete ui;
}

void FriendWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(198,198,198));
        this->setPalette(palette);
        emit friendInfo(friendId, nickname, signature, remark, headPic);

        isChoosed = true;
        emit isChoosedChanged(0);  //发送该模块已被选中的信号
    }
}

void FriendWidget::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void FriendWidget::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(232,232,232));
        this->setPalette(palette);
    }
}

bool FriendWidget::getIsChoosed()
{
    return this->isChoosed;
}

void FriendWidget::setIsChoosed(bool a)
{
    this->isChoosed = a;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(232,232,232));
    this->setPalette(palette);
}

void FriendWidget::setInfo(QString id, QString name, QString sex, QString sig, QString remark, QPixmap pic)
{
    friendId = id;
    nickname = name;
    this->sex = sex;
    signature = sig;
    this->remark = remark;
    headPic = pic;
    ui->name_label->setText(remark);
    QPixmap pix;
    pix = pic.scaled(40,40);
    ui->head_icon_label->setPixmap(pix);
}

void FriendWidget::setType(int t)
{
    type = t;
}

void FriendWidget::setRemark(QString remark)
{
    this->remark = remark;
    ui->name_label->setText(remark);
}

QString FriendWidget::getId()
{
    return friendId;
}

QString FriendWidget::getRemark()
{
    return remark;
}

QString FriendWidget::getNickname()
{
    return nickname;
}

QString FriendWidget::getSex()
{
    return sex;
}

QString FriendWidget::getSignature()
{
    return signature;
}

QPixmap FriendWidget::getPic()
{
    return headPic;
}

