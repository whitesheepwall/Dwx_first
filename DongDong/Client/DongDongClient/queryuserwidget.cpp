#include "queryuserwidget.h"
#include "ui_queryuserwidget.h"

QueryUserWidget::QueryUserWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryUserWidget)
{
    ui->setupUi(this);
}

QueryUserWidget::~QueryUserWidget()
{
    delete ui;
}

void QueryUserWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        emit userInfo(userId, remark, nickname, userSex, signature, headPic);
    }
}

void QueryUserWidget::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void QueryUserWidget::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(232,232,232));
        this->setPalette(palette);
    }
}

void QueryUserWidget::setInfo(QString id, QString remark, QString nickname, QString sex, QString sig, QPixmap pic)
{
    userId = id;
    this->remark = remark;
    this->nickname = nickname;
    userSex = sex;
    signature = sig;
    headPic = pic;

    if(relation)
        ui->name_label->setText(remark+"("+id+")");  //是好友就显示备注
    else
        ui->name_label->setText(nickname+"("+id+")");  //不是好友就显示昵称
    ui->content_label->setText(sig);
    if(sex == "男")ui->sex_label->setText("♂");
    else ui->sex_label->setText("♀");

    pic=pic.scaled(40,40);
    ui->head_icon_label->setPixmap(pic);
}

void QueryUserWidget::setNewRemark(QString newRemark)
{
    remark = newRemark;
    ui->name_label->setText(newRemark);
}

void QueryUserWidget::setRelation(bool rel)
{
    relation = rel;
}

bool QueryUserWidget::getRelation()
{
    return relation;
}
