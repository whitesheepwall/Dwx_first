#include "choosedfriend.h"
#include "ui_choosedfriend.h"

ChoosedFriend::ChoosedFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoosedFriend)
{
    ui->setupUi(this);

    //setAttribute(Qt::WA_DeleteOnClose);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(255,255,255));
    this->setPalette(palette);

    connect(ui->choose_label,&CancelSign::clicked,[=](){
        emit cancelChoose(id);
        //this->close();
    });
}

ChoosedFriend::~ChoosedFriend()
{
    delete ui;
}

void ChoosedFriend::setInfo(QString id, QString remark, QPixmap pic)
{
    this->id = id;
    this->remark = remark;
    this->pic = pic;
    ui->name_label->setText(remark);
    pic = pic.scaled(30,30);
    ui->pic_label->setPixmap(pic);
}
