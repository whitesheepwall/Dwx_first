#include "setinfomation.h"
#include "ui_setinfomation.h"
#include "QBuffer"
#include "QMessageBox"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

SetInfomation::SetInfomation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetInfomation)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose,false);
    fileName = "";

    connect(ui->maleBtn, &QRadioButton::toggled,[=](){
        sex = "男";
    });
    connect(ui->femaleBtn, &QRadioButton::toggled,[=](){
        sex = "女";
    });

    connect(ui->okBtn, &QPushButton::clicked,[=](){
        nickname = ui->nicknameEdit->text();
        signature = ui->signatureEdit->toPlainText();
        if(nickname.length()<1){
            QMessageBox::information(this,"提示","昵称不能为空");
            return;
        }

        emit newInfo(nickname, sex, signature);
        this->close();
    });

    connect(ui->changePicBtn,&QPushButton::clicked,[=](){
        fileName = QFileDialog::getOpenFileName(this, "选择文件", ".", "Image Files(*.jpg *.jpeg *.png *.bmp)");
        if (!fileName.isEmpty()) {
            qDebug() << "this is the file name " << fileName;
        //**************图片转QByteArray***************************
            QPixmap pixmap(fileName);
            QByteArray byteArray = QByteArray();
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer,"png");

        //**************QByteArray用base64加密转QString***********
            QString picStr = QString(byteArray.toBase64());

            emit newPic(picStr,pixmap);  //发送新图片到服务器

//            QPixmap p;
//            p.loadFromData(byteArray,"png");
//            bool res = p.save(QString("./Icon/%1.png").arg(id));//保存从数据库读取的照片到本地
//            if(!res){
//                qDebug()<<"图片保存失败";
//            }

            QPixmap pix;  //更改显示的图片
            pixmap = pixmap.scaled(40,40);
            ui->headPic_Label->setPixmap(pixmap);
        }
    });
}

SetInfomation::~SetInfomation()
{
    delete ui;
    qDebug()<<"change Info Window delete";
}

void SetInfomation::setInfo(QString name, QString sex, QString sig, QString id)
{
    this->nickname = name;
    this->sex = sex;
    this->signature = sig;
    this->id = id;
    ui->nicknameEdit->setText(name);
    ui->id_label->setText(id);
    if(sex == "男")
        ui->maleBtn->toggle();
    else
        ui->femaleBtn->toggle();
    ui->signatureEdit->setText(sig);
}

void SetInfomation::setPic(QPixmap headicon)
{
    ui->headPic_Label->setPixmap(headicon);
}



