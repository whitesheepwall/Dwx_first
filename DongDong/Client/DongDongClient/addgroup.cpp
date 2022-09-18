#include "addgroup.h"
#include "ui_addgroup.h"
#include <QGraphicsDropShadowEffect>


AddGroup::AddGroup(const QMap<QString,FriendWidget*> &friendWidget) :
    ui(new Ui::AddGroup)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    sumChoosed = 0;
    //*****************添加阴影***********************
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0,0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(12);
    this->setGraphicsEffect(shadow);

    connect(ui->cancelBtn,&QPushButton::clicked,[=](){
        this->close();
    });
    connect(ui->okBtn,&QPushButton::clicked,this,&AddGroup::returnResult);

    int index = 0;
    for(QMap<QString,FriendWidget*>::const_iterator it = friendWidget.begin();it!=friendWidget.end();it++){
        QString id = it.value()->getId();
        QString remark = it.value()->getRemark();
        QPixmap pic = it.value()->getPic();
        ChooseFriend *c = new ChooseFriend;
        c->setInfo(id,remark,pic);
        chooseFriendList.insert(id,c);
        ui->friendsLayout->insertWidget(index++,c);

        connect(c,&ChooseFriend::choosed,this,&AddGroup::choosed);
        connect(c,&ChooseFriend::cancelChoosed,this,&AddGroup::cancelChoosed);
    }
}

AddGroup::~AddGroup()
{
    delete ui;
}

void AddGroup::choosed(QString id,QString remark,QPixmap pic)
{
    ChoosedFriend *c = new ChoosedFriend;
    c->setInfo(id,remark,pic);
    choosedFriendList.insert(id,c);
    connect(c,&ChoosedFriend::cancelChoose,[=](QString id){
        cancelChoosed(id);
    });
    ui->choosedFriend_layout->insertWidget(sumChoosed,c);
    sumChoosed++;
    if(sumChoosed!=0){
        QString t = QString("     已添加%1个联系人").arg(sumChoosed);
        ui->label->setText(t);
    }
}

void AddGroup::cancelChoosed(QString id)
{
    chooseFriendList[id]->changeChooseState();
    QMap<QString,ChoosedFriend*>::Iterator it = choosedFriendList.find(id);
    if(it!=choosedFriendList.end()){
        delete it.value();
        it.value() = NULL;
        choosedFriendList.erase(it);
    }
    sumChoosed--;
    if(sumChoosed==0){
        ui->label->setText("     请勾选需要添加的联系人");
    }
    else{
        QString t = QString("     已添加%1个联系人").arg(sumChoosed);
        ui->label->setText(t);
    }
}

void AddGroup::returnResult()
{
    if(sumChoosed==0)return;
    QString member=QString::number(sumChoosed+1);
    for(QMap<QString,ChoosedFriend*>::Iterator it=choosedFriendList.begin();it!=choosedFriendList.end();it++){
        if(it.value()){
            member+=","+it.value()->id;
            delete it.value();
            it.value()=NULL;
        }
    }
    choosedFriendList.clear();
    for(QMap<QString,ChooseFriend*>::Iterator it=chooseFriendList.begin();it!=chooseFriendList.end();it++){
        if(it.value()){
            delete it.value();
            it.value()=NULL;
        }
    }
    chooseFriendList.clear();
    emit members(member);
    this->close();
}
