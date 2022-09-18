#include "loginandregister.h"
#include "ui_loginandregister.h"
#include <QRegExp>

LoginAndRegister::LoginAndRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginAndRegister)
{
    ui->setupUi(this);
    setFixedSize(400,300);
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->loginErrorLabel->hide();
    ui->loginAccountEdit->setText("xiaohong");
    ui->loginPasswordEdit->setText("123456");

    connect(ui->confirmBtn, &QPushButton::clicked, this, &LoginAndRegister::onLogin);
    connect(ui->registerBtn, &QPushButton::clicked, this, &LoginAndRegister::onRegister);
    connect(ui->cancelBtn, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(ui->cancelBtn_2, &QPushButton::clicked, [=](){
        this->close();
    });
}

LoginAndRegister::~LoginAndRegister()  //登录
{
    delete ui;
}

void LoginAndRegister::onLogin()
{
    QString account = ui->loginAccountEdit->text();
    QString password = ui->loginPasswordEdit->text();

    QRegExp regExp("^[0-9a-zA-Z]{6,12}$");
    if(!(regExp.exactMatch(account) && regExp.exactMatch(password))){
        ui->loginErrorLabel->setText("账号密码为6-12位大小写字母和数字组成");
        ui->loginErrorLabel->show();
        return;
    }

    emit loginInfo(account, password);
}

void LoginAndRegister::onRegister()  //注册
{
    QString account = ui->registerAccountEdit->text();
    QString password = ui->registerPasswordEdit->text();
    QString passwordAgain = ui->pwdAgainEdit->text();

    QRegExp regExp("^[0-9a-zA-Z]{6,12}$");
    if(!(regExp.exactMatch(account) && regExp.exactMatch(password))){
        ui->registerErrorLabel->setText("账号密码为6-12位大小写字母和数字组成");
        if(!ui->registerErrorLabel->isVisible())
            ui->registerErrorLabel->show();
        return;
    }
    if(password!=passwordAgain){
        ui->registerErrorLabel->setText("两次密码不一致");
        if(!ui->registerErrorLabel->isVisible())
            ui->registerErrorLabel->show();
        return;
    }

    emit registerInfo(account, password);
}

void LoginAndRegister::loginResult(bool res)  //服务器返回登录的结果
{
    if(res)
        this->close();
    else{
        ui->loginErrorLabel->setText("用户名或密码错误");
        if(!ui->loginErrorLabel->isVisible())
            ui->loginErrorLabel->show();
        ui->loginPasswordEdit->clear();
        ui->loginPasswordEdit->setFocus();
    }
}

void LoginAndRegister::registerResult(bool res)  //服务器返回注册的结果
{
    if(!res){
        ui->registerErrorLabel->setText("用户名已存在");
        if(!ui->registerErrorLabel->isVisible())
            ui->registerErrorLabel->show();
        ui->registerPasswordEdit->clear();
        ui->registerPasswordEdit->setFocus();
    }
    else{
        ui->registerErrorLabel->setText("注册成功");
        if(!ui->registerErrorLabel->isVisible())
            ui->registerErrorLabel->show();
    }
}

