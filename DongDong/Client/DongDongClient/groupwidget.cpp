#include "groupwidget.h"
#include "ui_groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(198,198,198));
        this->setPalette(palette);
        emit groupInfo(groupId,groupName,num,groupPic);
        isChoosed = true;
        emit isChoosedChanged(1);  //发送该模块已被选中的信号
    }
}

void GroupWidget::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void GroupWidget::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(232,232,232));
        this->setPalette(palette);
    }
}

bool GroupWidget::getIsChoosed()
{
    return this->isChoosed;
}

void GroupWidget::setIsChoosed(bool a)
{
    this->isChoosed = a;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(232,232,232));
    this->setPalette(palette);
}

void GroupWidget::setGroupInfo(QString id, QString name, int num, QPixmap pic)
{
    groupId = id;
    groupName = name;
    this->num = num;
    groupPic = pic;
    ui->name_label->setText(name+"("+QString::number(num)+")");
    QPixmap pix;
    pix = pic.scaled(40,40);
    ui->head_icon_label->setPixmap(pic);
}

void GroupWidget::setMember(QString id,User *member)
{
    groupMember.insert(id,member);
}


QString GroupWidget::getId()
{
    return groupId;
}

QString GroupWidget::getName()
{
    return groupName;
}

QPixmap GroupWidget::getPic()
{
    return groupPic;
}

