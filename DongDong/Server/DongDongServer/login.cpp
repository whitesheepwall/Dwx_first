#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->accountEdit->setText("admin");
    ui->passwordEdit->setText("123456");

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("登录");
    ui->accountEdit->setFocus();
    ui->loginErrorLabel->hide();
    connect(ui->cancelBtn, &QPushButton::clicked,[=](){
        this->close();
    });

    connect(ui->confirmBtn, &QPushButton::clicked,[=](){
        QString acc = ui->accountEdit->text();
        QString pwd = ui->passwordEdit->text();
        if(acc == "admin" && pwd == "123456"){
            emit managerLoginSuccess();
            this->close();
        }
        else{
            ui->loginErrorLabel->show();
            ui->passwordEdit->clear();
            ui->passwordEdit->setFocus();
        }
    });
}

Login::~Login()
{
    delete ui;
}
