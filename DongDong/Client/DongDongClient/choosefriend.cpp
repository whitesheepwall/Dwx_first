#include "choosefriend.h"
#include "ui_choosefriend.h"

ChooseFriend::ChooseFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseFriend)
{
    ui->setupUi(this);


    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255,255,255));
    this->setPalette(palette);

    isChoosed = false;

    pix.load(":/menu/res/menu/noChoosed.png");
    pix = pix.scaled(25,25);
    ui->choose_label->setPixmap(pix);
}

ChooseFriend::~ChooseFriend()
{
    delete ui;
}

void ChooseFriend::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(198,198,198));
        this->setPalette(palette);     
    }
    isChoosed = !isChoosed;
    if(isChoosed){
        pix.load(":/menu/res/menu/choosed.png");
        pix = pix.scaled(25,25);
        ui->choose_label->setPixmap(pix);
        emit choosed(id,remark,pic);
    }
    else{
        pix.load(":/menu/res/menu/noChoosed.png");
        pix = pix.scaled(25,25);
        ui->choose_label->setPixmap(pix);
        emit cancelChoosed(id);
    }
}

void ChooseFriend::enterEvent(QEnterEvent *e)
{
    if(e->type() == QEnterEvent::Enter && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(216,216,216));
        this->setPalette(palette);
    }
}

void ChooseFriend::leaveEvent(QEvent *e)
{
    if(e->type() == QEvent::Leave && !isChoosed){
        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, QColor(255,255,255));
        this->setPalette(palette);
    }
}

void ChooseFriend::setInfo(QString id, QString remark, QPixmap pic)
{
    this->id = id;
    this->remark = remark;
    this->pic = pic;
    ui->name_label->setText(remark);
    pic = pic.scaled(30,30);
    ui->pic_label->setPixmap(pic);
}

void ChooseFriend::changeChooseState()
{
    isChoosed = false;
    pix.load(":/menu/res/menu/noChoosed.png");
    pix = pix.scaled(25,25);
    ui->choose_label->setPixmap(pix);
}
