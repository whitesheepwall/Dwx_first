#include "setremarkwidget.h"
#include "ui_setremarkwidget.h"

SetRemarkWidget::SetRemarkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetRemarkWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint); //取消标题栏
    setWindowModified(true);

    connect(ui->okBtn, &QPushButton::clicked, [=](){
        QString str = ui->newRemarkEdit->text();
        emit newRemark(str);
        this->hide();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, [=](){
        this->hide();
    });
}

SetRemarkWidget::~SetRemarkWidget()
{
    delete ui;
}

void SetRemarkWidget::setOriginalRemark(QString oriRemark)
{
    ui->newRemarkEdit->setText(oriRemark);
}
