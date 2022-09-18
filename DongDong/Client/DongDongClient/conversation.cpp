#include "conversation.h"
#include "ui_conversation.h"

Conversation::Conversation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Conversation)
{
    ui->setupUi(this);
    setFixedSize(251,70);
    setAttribute(Qt::WA_StyledBackground);  //顶层窗口默认不生效，让顶层窗口样式生效
    this->setAutoFillBackground(true);
    isChoosed = false;
    remark = ui->name_label->text();
}

Conversation::~Conversation()
{
    delete ui;
}

void Conversation::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(198,198,198));
        this->setPalette(palette);
        emit friendInfo(id, remark, pic, type);
        isChoosed = true;
        emit isChoosedChanged();  //发送该模块已被选中的信号
    }
}

void Conversation::setInfo(QString id, QString remark, QPixmap pic, QString time, QString content)
{
    this->id = id;
    this->remark = remark;
    this->pic = pic;
    this->time = time;
    this->content = content;
    ui->content_label->setText(content);
    ui->name_label->setText(remark);
    ui->time_label->setText(time);
    pic = pic.scaled(40,40);
    ui->head_icon_label->setPixmap(pic);
}

void Conversation::setType(int t)
{
    type = t;
}

void Conversation::updateInfo(QString content, QString time)
{
    ui->content_label->setText(content);
    ui->time_label->setText(time);
}

void Conversation::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void Conversation::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(232,232,232));
        this->setPalette(palette);
    }
}

bool Conversation::getIsChoosed()
{
    return this->isChoosed;
}

void Conversation::setIsChoosed(bool a)
{
    this->isChoosed = a;
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(232,232,232));
    this->setPalette(palette);
}

QPixmap Conversation::getPic()
{
    return pic;
}
